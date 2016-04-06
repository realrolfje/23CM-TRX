
// returns rotary
int selectInt(String title, String suffix, int& current, int minimum, int maximum, int step) {
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("> "); lcd.print(title);
  lcd.setCursor(0,1); lcd.print("  "); lcd.print(current); lcd.print(suffix);

  /* --- Viewing the menu item --- */
  boolean select = false;
  while (!select) {
    readRSSI();                       // Mute/unmute audio based on squelch.
    byte push = getRotaryPush();
    if (push == 2) { return 0; }      // long push, exit menu
    if (push == 1) { select = true; } // Change this setting
    
    int turn = getRotaryTurn();
    Serial.println(turn);
    if (turn != 0) { return turn; }   // Next/previous menu item.
  }

  lcd.setCursor(0,0); lcd.print(" ");
  lcd.setCursor(0,1); lcd.print(">");
  
  /* --- Changing the menu item --- */
  while (0 == getRotaryPush()) { // until rotary is pushed again
    readRSSI();                  // Mute/unmute audio based on squelch.
    int turn = getRotaryTurn();
    if (turn != 0) {
      current = constrain(current + (turn * step), minimum, maximum);

      lcd.setCursor(2,1); lcd.print(current); lcd.print(suffix);
    }
  }
  
  return 0;
}

