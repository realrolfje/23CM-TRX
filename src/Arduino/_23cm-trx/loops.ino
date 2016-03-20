/*
 * 
 */
 byte loopVfo() {
  Serial.println("--- Loop: VFO ---");
  lcd.clear();
  setRxFreq(rxFreqHz);
  lcd.setCursor(13,0); lcd.print("VFO");    // Indicate unsaved changes

  // Write to EEPROM every 10 seconds;
  const int writeEvery = 10000;
  unsigned long nextEpromWrite = millis() + writeEvery;
  boolean unsavedChanges = false;

  while (1) {
    updateSmeterDisplay();
    
    /* Handle turning of the rotary encoder */
    long up = getRotaryTurn() * rasterHz;
    if (up != 0) {
      nextEpromWrite = millis() + writeEvery; // Hold off writes
      lcd.setCursor(15,0); lcd.print("0");    // Indicate unsaved changes
      unsavedChanges = true;
      rxFreqHz += up;
      setRxFreq(rxFreqHz);
    }
  
    /* Exit if rotary pushed */
    if (2 == getRotaryPush()) {
      writeAllToEEPROM();
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

    /* Write changes to EEPROM */
    if (unsavedChanges && millis() > nextEpromWrite) {
      writeAllToEEPROM();
      lcd.setCursor(15,0); lcd.print("O");
      unsavedChanges = false;
    }      
  }
}

byte loopMenu() {
  Serial.println("--- Loop: Menu ---");
  int menuitem = 0;
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
          menuitem = loopMenuRotary(menuitem);
          byte push = getRotaryPush();
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
          menuitem = loopMenuRotary(menuitem);
          byte push = getRotaryPush();
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
      default : 
        Serial.println("Menu item out of bounds.");
        menuitem = loopMenuRotary(menuitem);

    }
    // Exit menu, write all changes to EEPROM
    writeAllToEEPROM();
  }
  return LOOP_VFO;
}

int loopMenuRotary(int item) {
  return constrain(item += getRotaryTurn(),0,1);
}


