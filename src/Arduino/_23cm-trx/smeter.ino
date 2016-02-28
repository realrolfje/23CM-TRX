/*
 * Routines to sample the S signal and display an
 * S-meter on the dislay with fast attack and slow decay.
 */
unsigned long nextMeterUpdate = -1;
const unsigned long meterUpdateMillis = 50;

// 10 characters plus a null character 
// so we can use it as a proper string.
#define meterdisplaysize 10
char meterdisplay[11] = 
{ METER_CHAR_1,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_0,
  METER_CHAR_1};

int currentMeter = 0;

void testSMeterDisplay(int value) {
  unsigned long nowmillis = millis();
  
  if (nowmillis < nextMeterUpdate) {
    // No update yet.
    return;
  }

  updateFilteredMeter(value);
  buildMeterDisplayString();
  writeMeterDisplay(0,0);

  nextMeterUpdate = nowmillis + meterUpdateMillis;
}

void updateSmeterDisplay() {
  unsigned long nowmillis = millis();
  
  if (nowmillis < nextMeterUpdate) {
    // No update yet.
    return;
  }

  updateFilteredMeter(analogRead(SMETER));
  buildMeterDisplayString();
  writeMeterDisplay(0,0);

  nextMeterUpdate = nowmillis + meterUpdateMillis;
}

/* 
 *  Fast attack, slow decay update of meter. Relative
 *  to the number of times this method is called.
 */
void updateFilteredMeter(int sample) {
  
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

void buildMeterDisplayString() {
  int fullblocks = (currentMeter*meterdisplaysize)/1024;
  int lastchar  = ((currentMeter*meterdisplaysize)%1024)/3;
  for(int i=0; i<meterdisplaysize; i++){
    if (i<=fullblocks) {
      meterdisplay[i] = METER_CHAR_3;
    } else if (i==fullblocks+1) {
      switch(lastchar) {
        case 0: meterdisplay[i] = METER_CHAR_0;
        case 1: meterdisplay[i] = METER_CHAR_1;
        case 2: meterdisplay[i] = METER_CHAR_2;
      }
    } else {
      meterdisplay[i]=METER_CHAR_0;
    }
  }
}

void writeMeterDisplay(byte col, byte row) {
  lcd.setCursor(col,row);
  lcd.print(meterdisplay);
}

