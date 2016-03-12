/*
 * Routines to sample the S signal and display an
 * S-meter on the dislay with fast attack and slow decay.
 */
unsigned long nextMeterUpdate = 0;
const unsigned long meterUpdateMillis = 50;

// 10 characters plus a null character 
// so we can use it as a proper string.
#define meterdisplaysize 10

int currentMeter = 0;

void testSMeterDisplay(int value) {
  unsigned long nowmillis = millis();
  
  if (nowmillis < nextMeterUpdate) {
    // No update yet.
    return;
  }

  updateFilteredMeter(value);
  writeSMeter(1,0);

  nextMeterUpdate = nowmillis + meterUpdateMillis;
}

void updateSmeterDisplay() {
  unsigned long nowmillis = millis();
  
  if (nowmillis < nextMeterUpdate) {
    // No update yet.
    return;
  }

  updateFilteredMeter(readRSSI());
  writeSMeter(0,0);

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
                  : 6; /* slow decay  */

  /* Calculate the approximate average with the new sample */
  currentMeter = currentMeter + ((sample - currentMeter) / nrSamples);
}

/*
 * Reads the RSSI from the MD3362. A strong signal is 0, a weak 
 * signal is 5V so we need to tweak the values a bit.
 * This function returns:
 * 0    for weak signals
 * 1023 for strong signals
 */
int readRSSI() {
  return map(analogRead(SMETER),54, 1023, 1023,0);
}

void writeSMeter(int row, int pos) {
  lcd.setCursor(pos,row);
  
  const int maxbars = 10 * 3; // 10 characters with 3 vertical bars each

  // Calculate bars from s meter here  
  int displaybars = currentMeter * maxbars / 1024;

  for (int barindex = 0; barindex < maxbars; barindex +=3) {
    int thischar = min(displaybars,3);
    if (barindex == 0 && thischar == 0) {
      lcd.print((char) METER_CHAR_1);
    } else {
      switch (thischar) {
        case 3:  lcd.print((char) METER_CHAR_3); break;  // triple bar
        case 2:  lcd.print((char) METER_CHAR_2); break;  // double bar
        case 1:  lcd.print((char) METER_CHAR_1); break;  // single bar
        default: lcd.print((char) METER_CHAR_0); break;  // no bar
      }
    }
    displaybars -= 3;
  }  
  lcd.print((char) METER_CHAR_1);
}
