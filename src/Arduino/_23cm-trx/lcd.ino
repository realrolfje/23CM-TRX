#include <LiquidCrystal.h>

#define lcdBacklightPin 10

#define CHAR_0 0
#define CHAR_1 1
#define CHAR_2 2
#define CHAR_3 3

// initialize the library with the numbers of the interface pins
//                rs E  D4 D5 D6 D7
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define brightnessvalues_SIZE 8
byte brightnessvalues[brightnessvalues_SIZE] = {5,15,30,55,105,155,205,255};
byte lcdBacklightBrightness = brightnessvalues_SIZE - 1;

byte getBacklightBrightness() {
  return lcdBacklightBrightness;
}

byte setBacklightBrightness(byte brightnessIndex) {
  if ((brightnessIndex >= 0) && (brightnessIndex < brightnessvalues_SIZE)) {
    lcdBacklightBrightness = brightnessIndex;
  }
}

/*****************************************************************/
void incBacklightBrightness(){
  if (lcdBacklightBrightness < brightnessvalues_SIZE-1){
    lcdBacklightBrightness++;
  }
  analogWrite(lcdBacklightPin, brightnessvalues[lcdBacklightBrightness]);
//  EEPROMWriteBacklightValue();
}

/*****************************************************************/
void decBacklightBrightness(){
  if (lcdBacklightBrightness > 0){
    lcdBacklightBrightness--;
  }
  analogWrite(lcdBacklightPin, brightnessvalues[lcdBacklightBrightness]);
//  EEPROMWriteBacklightValue();
}

/*****************************************************************/
void initLCD(){
  pinMode(lcdBacklightPin,OUTPUT);
//  digitalWrite(lcdBacklightPin,HIGH);
  incBacklightBrightness();
  lcd.begin(16, 2);
  createLCDCharacters();
}

/*****************************************************************/
String twoDigits(int number){
  String digits = "";
  byte msd = number/10;
  byte lsd = number%10;
  digits += String(msd,DEC);
  digits += String(lsd,DEC);  
  return digits;
}

/*****************************************************************/
void createLCDCharacters(){
  createChar(CHAR_0,
    B00000,
    B10000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B10000);

  createChar(CHAR_1,
    B00000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000);

  createChar(CHAR_2,
    B00000,
    B10000,
    B10100,
    B10100,
    B10100,
    B10100,
    B10100,
    B10000);

  createChar(CHAR_3,
    B00000,
    B10000,
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B10000);
}

// Ugly trick because the "modern" C no longer supports
// inline temporary arrays.
// <sarcasm> This is of course WAY prettier and more concise </sarcasm>.
void createChar(int character, 
                byte b0,byte b1,byte b2,byte b3,
                byte b4,byte b5,byte b6,byte b7) {

  byte tempchar[8] = {b0, b1, b2, b3, b4, b5, b6, b7};
  lcd.createChar(character, tempchar);
}

