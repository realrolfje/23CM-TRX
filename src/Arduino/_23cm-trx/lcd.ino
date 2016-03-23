/*
 * All routines related to putting text on the
 * LCD panel and controlling the backlight.
 */
#define brightnessvalues_SIZE 8
const byte brightnessvalues[brightnessvalues_SIZE] = {5, 15, 30, 55, 105, 155, 205, 255};

//byte getBacklightBrightness() {
//  return lcdBacklightBrightness;
//}
//
//byte setBacklightBrightness(byte brightnessIndex) {
//  if ((brightnessIndex >= 0) && (brightnessIndex < brightnessvalues_SIZE)) {
//    lcdBacklightBrightness = brightnessIndex;
//  }
//}

void incBacklightBrightness(){
  if (lcdBacklightBrightness < brightnessvalues_SIZE-1){
    lcdBacklightBrightness++;
  }
  analogWrite(LCD_BACKLIGHT, brightnessvalues[lcdBacklightBrightness]);
//  EEPROMWriteBacklightValue();
}

void decBacklightBrightness(){
  if (lcdBacklightBrightness > 0){
    lcdBacklightBrightness--;
  }
  analogWrite(LCD_BACKLIGHT, brightnessvalues[lcdBacklightBrightness]);
//  EEPROMWriteBacklightValue();
}

void setupLCD(){
  pinMode(LCD_BACKLIGHT,OUTPUT);

  lcd.begin(16, 2);
  createSMeterCharacters();

  incBacklightBrightness();
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

