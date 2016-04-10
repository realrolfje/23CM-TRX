/*
 * Firmware for the PE1JPD 23cm TRX project.
 *
 * This is an Ardiuno version of the firmware for the 
 * popular PE1JPD 23cm Transceiver. Information on how
 * to build this transceiver and where to get the parts
 * are at: http://www.pe1jpd.nl/index.php/23cm_nbfm/
 * 
 * Given correct wiring, see https://github.com/realrolfje/23CM-TRX/tree/master/src/Arduino
 * you can run this software on both version 1 and
 * version 2 of the board.
 * 
 * Developed using Arduini IDE 1.6.7 on OSX 10.10.5 (Yosemite).
 * 
 * This software will give your 23cm Transceiver:
 * - An Arduino-based project, for easy modification.
 * - Slow-decay S-meter with scale and single digit readout
 * - VFO mode with settings menu (long-press the rotary)
 * - Memory mode with 10 memories (long-press for memory scanning)
 * - Bargraph mode, 5 MHz wide with last set Rx frequency
 *   at the center of the graph.
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
const byte SUBAUDIO      = 13;


/* Connections - LCD panel */
const byte LCD_RS        = 11;
const byte LCD_LE        = 12;
const byte LCD_D4        =  4;
const byte LCD_D5        =  5;
const byte LCD_D6        =  6;
const byte LCD_D7        =  7;
const byte LCD_BACKLIGHT = 10;

/* Global constants */
const byte VFO_MEMORY_LOCATION   = 10; // 0-9 are user memories, 10 is used for VFO mode.
const unsigned long minTcxoRefHz =  9000000;
const unsigned long maxTcxoRefHz = 20000000;

/* Loop structure */
const byte LOOP_VFO         = 0;
const byte LOOP_VFO_MENU    = 1;
const byte LOOP_MEMORY      = 2;
const byte LOOP_MEMORY_MENU = 3;
const byte LOOP_SPECTRUM    = 4;

/* Global variables and settings */
unsigned long tcxoRefHz     = 12800000;
unsigned long rasterHz      = 25000;
int  lcdBacklightBrightness = 255; // Min 0, max 255
int  squelchlevel           = 3;   // Min 0, max 9, see loop.ino
byte selectedMemory         = VFO_MEMORY_LOCATION; 
byte mode                   = LOOP_VFO;
byte lastSelectedMemory     = 0;

/* TRX related settings */
unsigned long rxFreqHz = 1298375000; // Defaults to PI2NOS
int subAudioIndex      = -1;         // -1 is no audio. See subaudio.ino.
int repeaterShiftIndex = 2;          // No shift. See PLL.ino



/* Includes and external libraries */
#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(LCD_RS, LCD_LE, 
                  LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  Serial.begin(115200);
  readGlobalSettings();
  readMemory(VFO_MEMORY_LOCATION);
  
  setupControls();
  setupLCD();
  setupPLL();
  setupSubAudio();
  setupControls();
  setupSmeter();
}

void loop() {  
  while (1) {
    switch(mode) {
      case LOOP_VFO:      mode = loopVfo();      break;
      case LOOP_VFO_MENU: mode = loopVfoMenu();  break;
      case LOOP_MEMORY:   mode = loopMemory();   break;
      case LOOP_MEMORY_MENU:   mode = loopMemoryMenu();   break;
      case LOOP_SPECTRUM: mode = loopSpectrum(); break;
      default: mode = LOOP_VFO;
    }

    // When we switch to VFO or MEMORY mode, store
    // general settings so that the Transceiver
    // powers up in the last mode we were in.
    switch(mode) {
      case LOOP_VFO:      
      case LOOP_MEMORY:
      writeGlobalSettings();
    }
  }
}

