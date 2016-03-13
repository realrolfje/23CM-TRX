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

/* Shared constants */

#define PLL_LE        A5
#define PLL_DATA      A4
#define PLL_CLK       A3
#define TX_ON         A2

#define SMETER        A1
#define MUTE          A0

#define lcdBacklightPin 10 

/* Includes and external libraries */
#include <LiquidCrystal.h>


/* Shared (global) variables */
// initialize the library with the numbers of the interface pins
//                rs E  D4 D5 D6 D7
LiquidCrystal lcd(11,12, 4, 5, 6, 7);


void setup() {
  Serial.begin(115200);
  initLCD();
  initPLL(25000);
//  setupSubAudio();
//  setTone(885);  // 88.5 Hz
  setRxFreq(1298375000);
  initSmeter();
}

void loop() {
  updateSmeterDisplay();
}
