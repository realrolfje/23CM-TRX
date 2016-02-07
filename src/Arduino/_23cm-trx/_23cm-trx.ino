/*
 * Firmware for the PE1JPD 23cm TRX project.
 *
 * This is an Ardiuno version of the firmware for the 
 * popular PE1JPD 23cm Transceiver. Information on how
 * to build this transceiver and where to get the parts
 * are at: http://www.pe1jpd.nl/index.php/23cm_nbfm/
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

 // 8MHz osc / 8 = 1MHz.
 
void setup() {
  setupSubAudio();
}

void loop() {
  // put your main code here, to run repeatedly:
  setTone(88.5);
}
