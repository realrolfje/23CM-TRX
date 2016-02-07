/*
 * Valid subaudio frequencies:
 * 67.0
 * 69.3
 * 71.9
 * 74.4
 * 77.0
 * 79.7
 * 82.5
 * 85.4
 * 88.5
 * 91.5
 * 94.8
 * 97.4
 * 100.0
 * 103.5
 * 107.2
 * 110.9
 * 114.8
 * 118.8
 * 123.0
 * 127.3
 * 131.8
 * 136.5
 * 141.3
 * 146.2
 * 151.4
 * 156.7
 * 162.2
 * 167.9
 * 173.8
 * 179.9
 * 186.2
 * 192.8
 * 203.5
 * 210.7
 * 218.1
 * 225.7
 * 233.6
 * 241.8
 * 250.3
 */
#include <TimerOne.h>

const byte subaudio_pin = 3;

void setupSubAudio() {
  Timer1.initialize();
  Timer1.attachInterrupt(flipAudioAubaudio_pin);

  pinMode(subaudio_pin, OUTPUT); 
  stopTone();
}

/* Flipping an internal bit is faster than flipping a port */
boolean audioBit=false;
void flipAudioAubaudio_pin(){
  audioBit != audioBit;
  digitalWrite(subaudio_pin, audioBit);
}

void setTone(float frequencyHz){
  long halfPeriodMicroSeconds = 1000000.0 / (frequencyHz*2);
  Timer1.setPeriod(halfPeriodMicroSeconds);
  Timer1.start();
}

void stopTone(){
  Timer1.stop();
}

