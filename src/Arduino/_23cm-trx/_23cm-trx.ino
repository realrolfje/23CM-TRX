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
 */
 
void setup() {
  setupSubAudio();
}

void loop() {
  // put your main code here, to run repeatedly:
  setTone(88.5);
}
