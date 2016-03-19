/*
 * Firmware for the PE1JPD 23cm TRX project.
 *
 * PLEASE NOTE: SOFTWARE IS UNTESTED AND HAS NEVER RUN
 * ON A 23CM TRANSCEIVER. THIS WARNING WILL BE REMOVED
 * AFTER TESTING.
 * 
 * This is an Ardiuno version of the firmware for the 
 * popular PE1JPD 23cm Transceiver. Information on how
 * to build this transceiver and where to get the parts
 * are at: http://www.pe1jpd.nl/index.php/23cm_nbfm/
 * 
 * This software is targeted at Board version 2.2, in the original
 * software indicated as BOARD2 on line 13 of 23nbfm21.c.
 * 
 * Developed using Arduini IDE 1.6.7 on OSX 10.10.5 (Yosemite).
 * 
 * This software will give your 23cm Transceiver:
 * - An Arduino-based project, for easy modification.
 * 
 * Arduino project on atmega with internal oscillator
 * http://waihung.net/using-internal-oscillator-on-atmega328p/
 * 
 * Creating a board definition with correct fuses and ISP:
 * http://www.open-electronics.org/arduino-isp-in-system-programming-and-stand-alone-circuits/
 * 
 */

/* Connections - User Controls */
const byte ROTARY_A      =  3;
const byte ROTARY_B      =  2;
const byte ROTARY_PUSH   =  9;
const byte PTT           =  8;

/* Connections - Hardware */
const byte PLL_LE        = A5;
const byte PLL_DATA      = A4;
const byte PLL_CLK       = A3;
const byte TX_ON         = A2;
const byte SMETER        = A1;
const byte MUTE          = A0;
const byte SUBAUDIO = 13;


/* Connections - LCD panel */
const byte LCD_RS        = 11;
const byte LCD_LE        = 12;
const byte LCD_D4        =  4;
const byte LCD_D5        =  5;
const byte LCD_D6        =  6;
const byte LCD_D7        =  7;
const byte LCD_BACKLIGHT = 10;

/* Global variables and settings */
unsigned long tcxoRefHz = 12800000;
unsigned long rasterHz  = 25000;
byte lcdBacklightBrightness = 7; // See lcd.ino
int mutelevel = 3;






/* Includes and external libraries */
#include <LiquidCrystal.h>
LiquidCrystal lcd(LCD_RS, LCD_LE, 
                  LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  Serial.begin(115200);
  setupControls();
  setupLCD();
  setupPLL(25000);
  setupControls();
  setupSubAudio();
//  setTone(885);  // 88.5 Hz
  setupSmeter();
}

void loop() {
  unsigned long freq = 1298375000;

  while (1) {    
    setRxFreq(freq);
    while (!isPTTPressed()){
      long up = getRotaryTurn() * rasterHz;
      if (up != 0) {
        freq += up;
        setRxFreq(freq);
      }
      
      updateSmeterDisplay();    
    }
    
    digitalWrite(MUTE, true);
    setTxFreq(freq - 28000000);
  
    lcd.setCursor(0,1);
    lcd.print("   TRANSMIT     ");
    while (isPTTPressed()) {
      // wait
    }
  }
}


boolean isPTTPressed(){
  return !digitalRead(PTT);
}


