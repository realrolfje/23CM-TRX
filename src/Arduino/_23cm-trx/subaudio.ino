/*
 * Sub-audio related settings and functions.
 * 
 * Usage: 
 * 
 * Call setupSubAudio(); once in your setup();
 * 
 * To output a 88.5 Hz subaudio tone during transmitting, 
 * set subAudioIndex to 8, and call ctcssTx(). This will
 * keep flipping the subaudio port in the correct frequency
 * until you let go of the PTT key.
 * 
 * Use freqTentHz[] as reference for valid subaudio tones.
 */

/* Valid subaudio frequencies, multiplied by 10. (885 means 88.5 Hz) */
/*         Index: 0     1     2     3     4     5     6     7     8     9 */
int ctcss[] = { 670,  693,  719,  744,  770,  797,  825,  854,  885,  915,
                948,  974, 1000, 1035, 1072, 1109, 1148, 1188, 1230, 1273,
               1318, 1365, 1413, 1462, 1514, 1567, 1622, 1679, 1738, 1799,
               1862, 1928, 2035, 2107, 2181, 2257, 2336, 2418, 2503 };

void setupSubAudio() {
  pinMode(SUBAUDIO, OUTPUT); 
}

void ctcssTx() {
  // Tx without subaudio
  if (subAudioIndex < 0) {
    while (isPTTPressed()) { delay(10); }
    return;
  }

  int frequencyTenthHz = ctcss[subAudioIndex];
  long halfperiodus = (5000000/frequencyTenthHz); // Half period in microSeconds
  halfperiodus -= 41; // Compensate for delay added by instructions
  
  boolean audioBit = false;
  while (isPTTPressed()) {
    audioBit = !audioBit;
    digitalWrite(SUBAUDIO, audioBit);
    delayMicroseconds(halfperiodus);
  }
  digitalWrite(SUBAUDIO, false);
}

void printCTCSS() { 
  if (subAudioIndex < 0) {
    lcd.print("Off         ");
  } else {
    lcd.print(ctcss[subAudioIndex]/10); lcd.print(".");
    lcd.print(ctcss[subAudioIndex]%10); lcd.print(" Hz ");
  }
}


