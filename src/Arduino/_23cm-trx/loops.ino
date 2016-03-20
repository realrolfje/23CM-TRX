/*
 * 
 */
 void loopVfo() {
  lcd.clear();
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
      startCTCSS();
      setTxFreq(rxFreqHz - 28000000);
  
      while(isPTTPressed()) {
        // wait
      }
    
      stopCTCSS();
      setRxFreq(rxFreqHz);
    }
  }
}

void loopMenu() {
  byte menuitem = 0;

  while(1) {   
    switch(menuitem) {
      case 0 : // Squelch menu
        lcd.clear();
        lcd.print((char) 0x10); lcd.print("Squelch level:");
        lcd.setCursor(1,0);
        lcd.print(" ");
        lcd.print(squelchlevel);

        while(menuitem == 0) {
          menuitem += getRotaryTurn();
          byte push = getRotaryPush();

          if (push == 2) { return; } // long push, exit
          if (push == 1) {
            // Menu item selected, rotary now selects squelch            
            lcd.setCursor(0,0); lcd.print(" ");
            lcd.setCursor(1,0); lcd.print((char) 0x10);
            
            while (0 == getRotaryPush()) { // until rotary is pushed again
              readRSSI();                  // Mute/unmute audio based on squelch.
              int turn = getRotaryTurn();
              if (turn != 0) {
                squelchlevel = constrain(squelchlevel + turn, 0, 9);                                
                lcd.setCursor(1,1);
                lcd.print(squelchlevel);
              }
            }
          }
        }
        break;
      default : menuitem = constrain(menuitem,0,1);
    }
  }
}

