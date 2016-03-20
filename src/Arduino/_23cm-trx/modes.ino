/*
 * 
 */
 void loopVfo() {
  setRxFreq(rxFreqHz);

  while (1) {
    updateSmeterDisplay();
    
    /* Handle turning of the rotary encoder */
    long up = getRotaryTurn() * rasterHz;
    if (up != 0) {
      rxFreqHz += up;
      setRxFreq(rxFreqHz);
    }
  
    /* Exit if rotary pushed */
    if (getRotaryPush()) {
      return;
    }
  
    /* Handle PTT */
    if (isPTTPressed()) {
      digitalWrite(MUTE, true);
      setTxFreq(rxFreqHz - 28000000);
  
      while(isPTTPressed()) {
        // wait
      }
    
      setRxFreq(rxFreqHz);
    }
  }
}

