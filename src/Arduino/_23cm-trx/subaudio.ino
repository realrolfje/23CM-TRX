/*
 * Sub-audio related settings and functions.
 * 
 * Usage: 
 * 
 * Call setupSubAudio(); once in your setup();
 * 
 * To output a 88.5 Hz subaudio tone, use setTone(885);
 * To stop subaudio output, use stopTone();
 * 
 * Use freqTentHz[] as reference for valid subaudio tones.
 */
#include <TimerOne.h>

const byte subaudio_pin = 3;

/* Valid subaudio frequencies, multiplied by 10. (885 means 88.5 Hz) */
int freqTenthHz[] = { 670,  693,  719,  744,  770,  797,  825,  854,  885,  915,
                      948,  974, 1000, 1035, 1072, 1109, 1148, 1188, 1230, 1273,
                     1318, 1365, 1413, 1462, 1514, 1567, 1622, 1679, 1738, 1799,
                     1862, 1928, 2035, 2107, 2181, 2257, 2336, 2418, 2503 };

void setupSubAudio() {
  pinMode(subaudio_pin, OUTPUT); 
  Timer1.initialize();
  Timer1.attachInterrupt(flipAudioAubaudioPin);

  stopTone();
}

/* Flipping an internal bit is faster than flipping a port */
volatile boolean audioBit=false;
void flipAudioAubaudioPin() {
  audioBit = !audioBit;
  digitalWrite(subaudio_pin, audioBit);
}

void setTone(int frequencyTenthHz) {
  /* Period corrected for 16MHz Arduino plus ISR timer inaccuracy */
  long halfPeriodMicroSeconds = 10012429 / (frequencyTenthHz * 2);
  Timer1.setPeriod(halfPeriodMicroSeconds);
  Timer1.start();
}

void stopTone(){
  Timer1.stop();
  audioBit = false;
  digitalWrite(subaudio_pin, audioBit);
}

