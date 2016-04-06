/*
 * All routines related to putting text on the
 * LCD panel and controlling the backlight.
 */

void setBacklightBrightness(int brightness){
  lcdBacklightBrightness = constrain(brightness, 0, 255);

  analogWrite(LCD_BACKLIGHT, brightness);
}

void setupLCD(){
  pinMode(LCD_BACKLIGHT,OUTPUT);

  lcd.begin(16, 2);
  createSMeterCharacters();

  // Todo: always on. Need to switch timers to make this work.
  analogWrite(LCD_BACKLIGHT, 255);
}

void displayFrequency(unsigned long freq){
  lcd.setCursor(5,0);
  lcd.print("000 MHz");
  lcd.setCursor(0,0);
  lcd.print(freq / 1000000);
  lcd.print(".");
  lcd.print((freq % 1000000) / 1000);
}

/* 
 *  Ugly trick because the "modern" C no longer supports
 * inline temporary arrays.
 *  
 *  <sarcasm> This is of course WAY prettier and more concise </sarcasm>.
 * 
 */
void createChar(int character, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h) {
  byte tempchar[8] = {a, b, c, d, e, f, g, h};
  lcd.createChar(character, tempchar);
}

