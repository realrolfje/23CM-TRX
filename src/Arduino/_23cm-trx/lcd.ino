
#define METER_CHAR_0 0 // :
#define METER_CHAR_1 1 // |
#define METER_CHAR_2 2 // ||
#define METER_CHAR_3 3 // |||

#define brightnessvalues_SIZE 8
byte brightnessvalues[brightnessvalues_SIZE] = {5,15,30,55,105,155,205,255};
byte lcdBacklightBrightness = brightnessvalues_SIZE - 1;

//byte getBacklightBrightness() {
//  return lcdBacklightBrightness;
//}
//
//byte setBacklightBrightness(byte brightnessIndex) {
//  if ((brightnessIndex >= 0) && (brightnessIndex < brightnessvalues_SIZE)) {
//    lcdBacklightBrightness = brightnessIndex;
//  }
//}
//
void incBacklightBrightness(){
  if (lcdBacklightBrightness < brightnessvalues_SIZE-1){
    lcdBacklightBrightness++;
  }
  analogWrite(lcdBacklightPin, brightnessvalues[lcdBacklightBrightness]);
//  EEPROMWriteBacklightValue();
}

void decBacklightBrightness(){
  if (lcdBacklightBrightness > 0){
    lcdBacklightBrightness--;
  }
  analogWrite(lcdBacklightPin, brightnessvalues[lcdBacklightBrightness]);
//  EEPROMWriteBacklightValue();
}

void setupLCD(){
  pinMode(lcdBacklightPin,OUTPUT);
  incBacklightBrightness();
  lcd.begin(16, 2);
  createLCDCharacters();
}

void displayFrequency(unsigned long freq){
  lcd.setCursor(5,0);
  lcd.print("000 MHz");
  lcd.setCursor(0,0);
  lcd.print(freq / 1000000);
  lcd.print(".");
  lcd.print((freq % 1000000) / 1000);
}

void createLCDCharacters(){

  createChar(METER_CHAR_0,
    B00000,
    B10000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B10000);

  createChar(METER_CHAR_1,
    B00000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000);

  createChar(METER_CHAR_2,
    B00000,
    B10000,
    B10100,
    B10100,
    B10100,
    B10100,
    B10100,
    B10000);

  createChar(METER_CHAR_3,
    B00000,
    B10000,
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B10000);
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

