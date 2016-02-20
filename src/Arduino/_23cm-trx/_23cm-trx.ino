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

void setup() {
  setupSubAudio();
  setTone(885);  // 88.5 Hz
}

void loop() {
  // put your main code here, to run repeatedly:
}
