/*
 * 23cm JPD transceiver
 * 
 * #	change								date		by
 * ------------------------------------------------------------
 * 1.0	initial version						01-01-15	pe1jpd
 * 1.5	software squelch					26-05-15	pe1jpd
 * 2.0	PORTC change						01-07-15	pe1jpd
 * 2.1	PORTC bug solved					19-08-15	pe1jpd
 * 2.2	fref saved/restored from eeprom		02-10-15	pe1jpd
 */

#define BOARD2
#define version		"2.2"

#define F_CPU 		1000000UL

#define F_RASTER 	25			// in kHz
#define	IF			69300UL		// in kHz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

typedef unsigned char u08;

#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

#define FALSE 		0
#define TRUE		!FALSE
#define SHORT		1
#define	LONG		2

#define MUTEVAL		0
#define FSHIFT		1
#define CTCSS		2
#define FREF		3
#define BACK		4

// various
#ifdef BOARD2
#define Smeter		PC5
#define MUTE		PC4
#define TXON		PC3
#define LE	     	PC2
#define DATA    	PC1
#define CLK     	PC0
#endif

#ifdef BOARD1
#define Smeter		PC1
#define MUTE		PC0
#define TXON		PC2
#define LE	     	PC5
#define DATA    	PC4
#define CLK     	PC3
#endif

// rotary & switches
#define PTT			PD0
#define SHIFTKEY	PD1
#define PUSH		PD2
#define ROTINT		PD3
#define	ROT			PD4

// LCD
#define	LCD_D7		PB7
#define	LCD_D6		PB6
#define	LCD_D5		PB5
#define	LCD_D4		PB4
#define	LCD_RS		PB1
#define	LCD_E		PB0

#define Beep		PB3

// function prototypes
void initPLL(long int freq);
void setPLL(long int r);
void setFreq(long int freq);
void display(long int f);
void initLcd();
void lcdCmd(char c);
void lcdData(char c);
void lcdStr(char *s);
void doMenu();
void menuShift();
int readSmeter();

// variables
long int freq;
long int shift;
int fref;
int muteval;
int shiftSwitch = FALSE;
int step = F_RASTER;
int menu;
int lps;
int tx = FALSE;
int enc = 0;
char str[16];
u08 buf[8];
int tick;

// define menus
char *menuStr[5] = {
	"> Set mutelevel:",
	"> Set shift:    ",
	"> Set tone:     ",
	"> Set reference:",
	"> Back          "
};

// define S-meter chars
unsigned char smeter[3][8] = {
	{0b00000,0b00000,0b10000,0b10000,0b10000,0b10000,0b00000,0b00000},
	{0b00000,0b00000,0b10100,0b10100,0b10100,0b10100,0b00000,0b00000},
	{0b00000,0b00000,0b10101,0b10101,0b10101,0b10101,0b00000,0b00000}
};

// CTCSS sine table
unsigned char sample;
unsigned char sintbl[] = {
		0x80,0x98,0xb0,0xc7,0xda,0xea,0xf6,0xfd,
		0xff,0xfd,0xf6,0xea,0xda,0xc7,0xb0,0x98,
		0x80,0x67,0x4f,0x38,0x25,0x15,0x09,0x02,
		0x00,0x02,0x09,0x15,0x25,0x38,0x4f,0x67
};
int tone;
long int toneCount;

// Interrupt service routine INT1
ISR(INT1_vect)
{
	if (PIND & (1<<ROT))
 		enc++;
	else 
		enc--;
	_delay_ms(10);
}

// Interrupt service routine Timer1
ISR(TIMER1_OVF_vect) 
{ 
	tick++;						// increment tick for freq save timeout
	TCNT1 = 65536-toneCount;	// restart timer

	if (tx && tone>650)
		tbi(PORTB, Beep);		// toggle Beep port

//	if (tx) {
//	    OCR2A = sintbl[sample];
//		sample += 2;
//		if (sample > 31)
//			sample = 0;	// 32 samples in sintbl
//	}
} 

void initADC(void) 
{ 
	ADCSRA = (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);  // prescaler /128
#ifdef BOARD2
	DIDR0 = (1<<ADC5D); 		// disable digital input
	ADMUX = (1<<REFS0) + 5;		// ADC channel 5 
#endif
#ifdef BOARD1
	DIDR0 = (1<<ADC0D); 		// disable digital input
	ADMUX = (1<<REFS0) + 1;		// ADC channel 1 
#endif
} 

void initInterrupts(void)
{
	EIMSK |= _BV(INT1);				// enable INT1
	EICRA |= _BV(ISC11);			// int1 on falling edge

	// Setup Timer 1
	TCCR1A = 0x00;					// Normal Mode 
	TCCR1B = 0x01;					// div/1 clock, 1/F_CPU clock

	// Setup Timer 2 for PWM
//    TCCR2A  = (1<<COM2A1); 			// non inverting mode
//	TCCR2A |= (1<<WGM21)|(1<<WGM20);// CLC mode
//  TCCR2B  = (1<<CS21);			// prescaler = 8

	// Enable interrupts as needed 
	TIMSK1 |= _BV(TOIE1);  			// Timer 1 overflow interrupt 

	// enable interrupts
	sei();
}

void hex2bcd(long int f)
{
	long int x = 10000000;
	int i;
	char d;

	for (i=0; i<8; i++) {
		d = 0;
		for (;;) {
			f -= x;
			if (f < 0) break;
			d++;
		}
		str[i] = d + 0x30;
		f += x;
		x = x/10;
	}
}

void display(long int f)
{
	int i;
	char c;

	hex2bcd(f);

	lcdCmd(0x80 + 4);
	_delay_us(200);

	for (i=1; i<8; i++) {
		c = str[i];
		if (i==5) {
			lcdData('.');
		}
		lcdData(c);
	}
}

void initLcd()
{
	int i,j;

	// 4 bits mode, 2 lines
	lcdCmd(0x20);
	_delay_ms(10);
	
	lcdCmd(0x20);
	_delay_ms(10);
	
	// cursor shift right, text no shift
	lcdCmd(0x18);
	_delay_ms(20);

	// display on, no cursor, no blink
	lcdCmd(0x0c);
	_delay_ms(20);

	// shift mode
	lcdCmd(0x06);
	_delay_ms(20);

	// home
	lcdCmd(0x02);
	_delay_ms(20);

	// clear display
	lcdCmd(0x01);
	_delay_ms(20);

	// define custom chars
	lcdCmd(0x40);
	for (i=0; i<3; i++) {
		for (j=0; j<8; j++) {
			lcdData(smeter[i][j]);
		}
	}
}

void lcdCmd(char c)
{
	char t;

	t = c & 0xf0;
	PORTB = t;
	sbi(PORTB, LCD_E);
	_delay_us(2);
	cbi(PORTB, LCD_E);
	_delay_us(200);

	c <<= 4;
	c &= 0xf0;
	PORTB = c;
	sbi(PORTB, LCD_E);
	_delay_us(2);
	cbi(PORTB, LCD_E);
	_delay_us(200);
}

void lcdData(char c)
{
	char t;

	t = c & 0xf0;	
	t |= (1<<LCD_RS);	
	PORTB = t;
	sbi(PORTB, LCD_E);
	_delay_us(2);
	cbi(PORTB, LCD_E);
	_delay_us(200);

	c <<= 4;
	c &= 0xf0;
	c |= (1<<LCD_RS);	
	PORTB = c;
	sbi(PORTB, LCD_E);
	_delay_us(2);
	cbi(PORTB, LCD_E);
	_delay_us(200);
}

void lcdStr(char *s)
{
	while (*s) 
		lcdData(*s++);
}

void update()
{
	long int f;

	if (tx) {
		f = freq;
		if (shiftSwitch)
			f += shift;
		setFreq(f);
		display(f);
		lcdCmd(0x8f);
		lcdData('T');
	}
	else {
		setFreq(freq - IF);
		display(freq);
		lcdCmd(0x8f);
		lcdData('R');
	}
}

int rotaryPushed()
{
	int i, c;

	c = PIND;

	// rotary button pushed?
	if (!(c & (1<<PUSH))) {
		for (i=0;;i++) {
			_delay_ms(200);
			// wait for button released
			if ((PIND & (1<<PUSH))) {
				_delay_ms(200);
				break;
			}
		}
		if (i>5)
			return LONG;
		else
			return SHORT;
	}
	return FALSE;
}

int handleRotary()
{
	int count=0;

	if (enc != 0) {
		if (enc>0) {
			enc--;
			count++;
		}
		if (enc<0) {
			enc++;
			count--;
		}
	}
	return count;
}

void doMenu()
{
	int s, c=1, quit = FALSE;

	menu = 0;

	do {

		// first time and at an update display the value
		if (c!=0) {

			lcdCmd(0x80);
			lcdStr(menuStr[menu]);

			switch (menu) {
				case MUTEVAL:
				sprintf(str, "  %d        ", muteval);
				break;

				case FSHIFT:
				sprintf(str, "  %+2ld MHz        ", shift/1000);
				break;

				case CTCSS:
				if (tone>650)
					sprintf(str, "  %d.%01d Hz      ", tone/10, tone-(tone/10)*10 );
				else
					sprintf(str, "  off            ");
				break;

				case FREF:
				sprintf(str, "  %d.%01d MHz        ", fref/1000, fref-(fref/1000)*1000);
				break;

				default:
				sprintf(str, "                ");
				break;
			}

			lcdCmd(0xc0);
			lcdStr(str);
		}

		// handle encoder
		c = handleRotary();
		if (c!=0) {
			menu += c;
			if (menu>4) menu = 0;
			if (menu<0) menu = 4;
		}

		if (rotaryPushed()) {

			lcdCmd(0x80);
			lcdData(' ');
			lcdCmd(0xc0);
			lcdData('>');

			switch (menu) {

				case MUTEVAL:
				lcdCmd(0xc5);
				for (;;) {
					c = handleRotary();
					if (c!=0) {
						if (c>0) {
							if (++muteval > 100) muteval = 100;
						}
						else { 
							if (--muteval < 0) muteval = 0;
						}
						lcdCmd(0xc0);
						sprintf(str, "> %d  ", muteval);
						lcdStr(str);

						s = readSmeter();
						if (s > muteval)
							cbi(PORTC, MUTE);
						else
							sbi(PORTC, MUTE);
					}

					if (rotaryPushed()) {
						eeprom_write_word((unsigned int *)0x0c, muteval);
						break;
					}
				}
				break;

				case FSHIFT:
				lcdCmd(0xc5);
				for (;;) {
					c = handleRotary();
					if (c!=0) {
						if (c>0) {
							shift += 1000;
							if (shift>60000) shift = 60000;
						}
						else {
							shift -= 1000;
							if (shift<-60000) shift = -60000;
						}

						lcdCmd(0xc0);
						sprintf(str, "> %+2ld MHz  ", shift/1000);
						lcdStr(str);
					}

					if (rotaryPushed()) {
						eeprom_write_word((unsigned int *)0x18, shift);
						break;
					}
				}
				break;

				case CTCSS:
				lcdCmd(0xc5);
				for (;;) {
					c = handleRotary();
					if (c!=0) {
						if (c>0) {
							tone++;
						}
						else {
							tone--;
							if (tone<650) tone = 650;
						}

						lcdCmd(0xc0);
						if (tone>650)
							sprintf(str, "> %d.%01d Hz      ", tone/10, tone-(tone/10)*10 );
						else
							sprintf(str, "> off            ");
						lcdStr(str);
						toneCount = 5*F_CPU/tone;	// *10/2
						// T1 counter, Hz*10, 16 samples/period
//						toneCount = 10*F_CPU/(tone*16);
					}

					if (rotaryPushed()) {
						eeprom_write_word((unsigned int *)0x1c, tone);
						break;
					}
				}
				break;

				case FREF:
				lcdCmd(0xc5);
				for (;;) {
					c = handleRotary();
					if (c!=0) {
						if (c>0) {
							fref += 100;
						}
						else {
							fref -= 100;
						}

						lcdCmd(0xc0);
						sprintf(str, "> %d.%01d MHz  ", fref/1000, fref-(fref/1000)*1000 );
						lcdStr(str);
					}

					if (rotaryPushed()) {
						eeprom_write_word((unsigned int *)0x00, fref);
					    initPLL(freq - IF);
						break;
					}
				}
				break;

				case BACK:
				quit = TRUE;

				default:
				break;
			}

			lcdCmd(0x80);
			lcdData('>');
			lcdCmd(0xc0);
			lcdData(' ');
		}
	} while (!quit);
}

int readSmeter()
{
	register int s;

	ADCSRA |= (1<<ADSC)|(1<<ADEN); 
	while ((ADCSRA & (1<<ADSC))!=0);

	// calculate Smeter
	s = (1024-ADC)-44;
	if (s<1) s=1;

	// low pass s-meter signal
	s += lps;
	s >>= 1;
	lps = s;

	return s;
}

void displaySmeter(int level) 
{
	short n = 16;

	lcdCmd(0xc0);

	// chars in the full bar are 3 vertical lines
	while(level>=3 && n>0) {
		lcdData(2);
		level -= 3;
		n--;
	}
	
	// last char 0, 1 or 2 lines
	switch (level) {
		case 2: lcdData(1); break;
		case 1: lcdData(0); break;
		default: lcdData(' '); break;
	}

	// clear any chars to the right
	while (--n > 0) lcdData(' ');
}

int main()
{
	long int savedfreq = 0;
	int s, c;
	unsigned char sw=0;

	// PORTB output for LCD
	DDRB = 0xff;
	PORTB = 0xff;

#ifdef BOARD2
	// PORTC PC0-4 output, PC5 input
	DDRC = 0x1f;
	PORTC = 0x00;
	sbi(PORTC, MUTE);
#endif
#ifdef BOARD1
	// PORTC PC0,2-5 output, PC1 input
	DDRC = 0x3d;
	PORTC = 0x00;
	sbi(PORTC, MUTE);
#endif

	// PORTD is input with pullup
	DDRD = 0x00;
	PORTD = 0xff;

	initLcd();
	initADC();

	// set reference freq
	fref = eeprom_read_word((unsigned int *)0x00);
	if (fref < 2000 || (fref % 100) != 0) {
		fref = 12000;
		eeprom_write_word((unsigned int *)0x00, fref);
	}

	// read squelch level from eeprom
	muteval = eeprom_read_word((unsigned int *)0x0c);
	if (muteval < 0 || muteval > 100) {
		muteval = 0;
		eeprom_write_word((unsigned int *)0x0c, muteval);
	}

	// read last frequency from eeprom
	freq = eeprom_read_dword((unsigned long int *)0x10);
	if (freq < 1240000UL || freq > 1300000UL) {
		freq = 1298375UL;
		eeprom_write_dword((unsigned long int *)0x10, freq);
	}

	// read shift from eeprom
	shift = eeprom_read_word((unsigned int *)0x18);
	if (shift < 60000UL || shift > 60000UL) {
		shift = -28000UL;
		eeprom_write_word((unsigned int *)0x18, shift);
	}

	// read tone (*10) from eeprom
	tone = eeprom_read_word((unsigned int *)0x1c);
	if (tone < 650 || tone > 1500) {
		tone = 650;
		eeprom_write_word((unsigned int *)0x1c, tone);
	}

	initInterrupts();
    initPLL(freq - IF);
	update();

	sprintf(str, "JPD 23cm v%s", version);
	lcdCmd(0x80);
	lcdStr(str);
	_delay_ms(500);

	for (;;) {

		lcdCmd(0x80);
		lcdStr("VFO             ");
		lcdCmd(0xc0);
		lcdStr("                ");
		update();

		for (;;) {
			// read switches on PORTD
			sw = PIND;

			// switch from tx to rx??
			if (tx && (sw & (1<<PTT) )) {
				cbi(PORTC, TXON);
				// switch TX off
				tx = FALSE;
//  				TCCR2A  &= ~(1<<COM2A1);
				update();
			}

			// switch from rx to tx?
			else if (!tx && !(sw & (1<<PTT) )) {
				tx = TRUE;
				displaySmeter(0);
				// switch TX on
				sbi(PORTC, TXON);
				sbi(PORTC, MUTE);
//				if (tone > 650) {
//	   				TCCR2A  |= (1<<COM2A1);
//				}
				update();
			}

			if (!tx) {
				s = readSmeter();
				displaySmeter(s);
				if (s > muteval)
					cbi(PORTC, MUTE);
				else
					sbi(PORTC, MUTE);
			}

			// switch shift off
			if ( (shiftSwitch == TRUE) && (sw & (1<<SHIFTKEY) )) {
				shiftSwitch = FALSE;
				update();
			}
			// switch shift on
			else if ( (shiftSwitch == FALSE) && !(sw & (1<<SHIFTKEY) )) {
				shiftSwitch = TRUE;
				update();
			}

			// save vfo frequency in eeprom after ~2 secs inactivity
			if (tick > 200) {
				if (freq != savedfreq) {
					eeprom_write_dword((unsigned long int *)0x10, freq);
					savedfreq = freq;
				}
			}
	
			// handle encoder pulses
			c = handleRotary();
			if (c!=0) {
				if (c>0) {
					freq += step;
				}
				else {
					freq -= step;
				}
				tick = 0;
				update();
			}

			if (rotaryPushed()) {
				doMenu();
				break;
			}
		}
	}
}

void initPLL(long int f)
{
    long int reg;

    cbi(PORTC, DATA);
    cbi(PORTC, CLK);
    cbi(PORTC, LE);

	// set function latch
	reg = 0x438086;
    setPLL(reg);
    
    // init R-counter
	reg = (2UL<<16) + ((fref/F_RASTER)<<2);
    setPLL(reg);

	setFreq(f);

	reg = 0x438082;
	setPLL(reg);
}

void setFreq(long int f)
{
	long int reg, frast, A, B;
    
	frast = f/F_RASTER;
	B = frast/16;
	A = frast%16;
    
	reg = ((B & 0x1fff)<<8) + ((A & 0x3f)<<2) + 1;
    
    setPLL(reg);
}

void setPLL(long int r)
{
    int i;
    
    for (i=0; i<24; i++) {
        if (r & 0x800000)
            sbi(PORTC, DATA);
        else
            cbi(PORTC, DATA);
		_delay_us(1);
        sbi(PORTC, CLK);
        _delay_us(1);
        cbi(PORTC, CLK);
        r <<= 1;
    }
	_delay_us(1);
    sbi(PORTC, LE);
    _delay_us(1);
    cbi(PORTC, LE);
}


