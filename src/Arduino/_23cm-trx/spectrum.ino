/*
 * Spectrum scanner/analyzer
 */

/* Maps values (the index) to characters (the value) */
byte graphCharacterMap[9] =  { ' ', 0, 1, 2, 3, 4, 5, 6, 7 };
unsigned long scanWidthHz = 5000000;

byte loopSpectrum() {
  Serial.println("--- Loop: Sepctrum graph ---");

  lcd.clear();
  createGraphCharacters();
  
  unsigned long startFreq = rxFreqHz - scanWidthHz / 2;
  unsigned long endFreq   = rxFreqHz + scanWidthHz / 2;
  unsigned long step = (endFreq - startFreq) / 16;
  
  digitalWrite(MUTE, true);

  while (1){
    for (int x=0; x<16; x++) {
      lcd.setCursor(x,0); // top row
      lcd.print(".");     // Show where we are

      int signal = avgRSSI(startFreq + x * step,
                           startFreq + step + x * step);

      if (getRotaryPush() || signal < 0) {
        Serial.println(int(signal));
        return LOOP_VFO;
      }
                           
      byte y = signal * 16 / 1024; /* Bottom to top */

      lcd.setCursor(x,0); // top row
      if (y>8) { lcd.print((char)graphCharacterMap[min(8,y-8)]); } 
      else { lcd.print((char)graphCharacterMap[0]); }

      lcd.setCursor(x,1); // bottom row
      lcd.print((char)graphCharacterMap[min(8,y)]);
    }
  }
}

int avgRSSI(unsigned long startFreq, unsigned long endFreq) {
  int rssi = 0;
  for (unsigned long f = startFreq; f<endFreq; f += rasterHz) {
    if (getRotaryPush()) { return -1; };
      setVCOFreq(f - IF); // See setRxFreq() in PLL.ino
      delay(20);
      rssi = constrain(max(rssi, readRSSI(false)),0,1023);
  }
  if (rssi < 0) return {0}; // TODO: Workaround. Figure out why we can get negative numbers here.
  return rssi;
}


void createGraphCharacters() {
  createChar(0,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111);
  
  createChar(1,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111);
  
  createChar(2,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111);
  
  createChar(3,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111);
  
  createChar(4,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111);
  
  createChar(5,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111);
  
  createChar(6,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111);
  
  createChar(7,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111);
}

