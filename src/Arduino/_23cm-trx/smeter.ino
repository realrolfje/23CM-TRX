/*
 * Routines to sample the S signal and display an
 * S-meter on the dislay with fast attack and slow decay.
 */

#define SMETER    PC2

unsigned long nextMeterUpdate = -1;
const unsigned long meterUpdateMillis = 50;

// 10 characters plus a null character 
// so we can use it as a proper string.
#define meterdisplaysize 10
char meterdisplay[11] = "          ";

int currentMeter = 0;

void updateSmeterDisplay() {
  unsigned long nowmillis = millis();
  
  if (nowmillis < nextMeterUpdate) {
    // No update yet.
    return;
  }

  readFilteredMeter();

  int fullblocks = (currentMeter*meterdisplaysize)/1024;
  int lastchar  = ((currentMeter*meterdisplaysize)%1024)/3;
  for(int i=0; i<meterdisplaysize; i++){
    if (i<=fullblocks){
      meterdisplay[i]= 'X';
    } else {
      // TODO use the nice bar digits here
      // TODO one of the digits is 3 bars (lastchar).
      meterdisplay[i]= ' ';
    }   
  }

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


