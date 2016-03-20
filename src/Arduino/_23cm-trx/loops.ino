/*
 * 
 */
 byte loopVfo() {
  Serial.println("--- Loop: VFO ---");
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
    if (2 == getRotaryPush()) {
      return LOOP_MENU;
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

byte loopMenu() {
  Serial.println("--- Loop: Menu ---");
  byte menuitem = 0;
  boolean exit = false;

  while(!exit) {   
    switch(menuitem) {
      // --------------------------------------------------- Squelch menu
      case 0 : 
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("> Squelch level");
        lcd.setCursor(2,1); lcd.print((int)squelchlevel);
        
        while(!exit && menuitem == 0) {
         readRSSI();                  // Mute/unmute audio based on squelch.

          byte push = getRotaryPush();
          menuitem += getRotaryTurn();

          if (push == 2) { exit = true; } // long push, exit
          if (push == 1) {
            lcd.setCursor(0,0); lcd.print(" ");
            lcd.setCursor(0,1); lcd.print(">"); // Rotary now selects squelch            
            
            while (0 == getRotaryPush()) { // until rotary is pushed again
              readRSSI();                  // Mute/unmute audio based on squelch.
              int turn = getRotaryTurn();
              if (turn != 0) {
                squelchlevel = constrain(squelchlevel + turn, 0, 9);                                
                lcd.setCursor(2,1);
                lcd.print((int)squelchlevel);
              }
            }
            exit = true;
          }
        }
        break;
      // ------------------------------------------------ Subaudio/CTCSS menu
      case 1 : 
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("> Subaudio tone");
        lcd.setCursor(2,1); printCTCSS();

        while(!exit && menuitem == 1) {
          readRSSI();                  // Mute/unmute audio based on squelch.
          byte push = getRotaryPush();
          menuitem += getRotaryTurn();
          if (push == 2) { exit = true; } // long push, exit
          if (push == 1) {
            lcd.setCursor(0,0); lcd.print(" ");
            lcd.setCursor(0,1); lcd.print(">"); // Rotary now selects tone
            while (0 == getRotaryPush()) { // until rotary is pushed again
              readRSSI();                  // Mute/unmute audio based on squelch.
              int turn = getRotaryTurn();
              if (turn != 0) {
                subAudioIndex = constrain(subAudioIndex + turn, -1, 38); // See subaudio.ino
                lcd.setCursor(2,1); printCTCSS();
              }
            }
            exit = true;
          }
        }
        break;
      // ------------------------------------------------ Out of bounds.
      default : menuitem = constrain(menuitem,0,1);
    }
    writeAllToEEPROM();
  }
  return LOOP_VFO;
}


