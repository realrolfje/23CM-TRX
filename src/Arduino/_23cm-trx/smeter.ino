/*
 * Routines to sample the S signal and display an
 * S-meter on the dislay with fast attack and slow decay.
 */

#define SMETER    PC2

 // define S-meter chars
unsigned char smeter[3][8] = {
  {0b00000,
   0b00000,
   0b10000,
   0b10000,
   0b10000,
   0b10000,
   0b10000,
   0b00000},
   
  {0b00000,
   0b00000,
   0b10000,
   0b10000,
   0b10100,
   0b10100,
   0b10100,
   0b00000},
   
  {0b00000,
   0b00000,
   0b10000,
   0b10000,
   0b10101,
   0b10101,
   0b10101,
   0b00000}
};

unsigned long nextMeterUpdate = -1;
const unsigned long meterUpdateMillis = 50;

int currentMeter = 0;

void updateSmeterDisplay() {
  unsigned long nowmillis = millis();
  
  if (nowmillis < nextMeterUpdate) {
    // No update yet.
    return;
  }

  readFilteredMeter();
    
  nextMeterUpdate = nowmillis + meterUpdateMillis;
}

/* 
 *  Fast attack, slow decay update of meter. Relative
 *  to the number of times this method is called.
 */
void readFilteredMeter() {
  int sample = analogRead(SMETER);
  
  /* 
   * The fictive number of samples in our buffer determines
   * the realtive weight of the new sample.
   *  - When sample is bigger than the current meter reading, we
   *    simulate a small window for faster attack.
   *  - When sample is smaller than the current meter reading,
   *    we simulate a bigger window (more samples) for a slower
   *    decay.
   */
  int nrSamples = (sample > currentMeter) 
                  ? 1    /* fast attack */  
                  : 100; /* slow decay  */

  /* Calculate the approximate average with the new sample */
  currentMeter = currentMeter + ((sample - currentMeter) / nrSamples);
}


