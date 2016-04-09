/*
 * All code to store and load settings to EEPROM.
 *
 * Please note that the EEPROM storage is limited:
 * ATmega168  512 B - Arduino Nano 2.3
 * ATmega328 1024 B - Arduino Nano 3.0 and Pro Mini
 * 
 * The CRC check was replaced by a "magic number".
 * Although a CRC is much better for detecting garbled
 * memory and recovering from that, it also means that
 * the CRC bytes are written each time a setting
 * changes. This means that all efforts to only write
 * changes to prevent EEPROM fatique is useless, as
 * the CRC bytes will always be rewritten. The Arduino's
 * have a guaranteed write lifetime of 100.000 cycles,
 * which is not much.
 * 
 * Whenever the code for saving/loading the setting
 * changes, please chooose a different magic number.
 * 
 * Writing a single byte to EEPROM costs 3.3mS.
 * 
 */

const unsigned int magicnumber = 31342;
unsigned int eeprompointer = 0;

// Read global settings
void readGlobalSettings() {
  eeprompointer = 0;
  if (!checkMagicNumber()) { return; } // Exit if no magic

  tcxoRefHz              = EEPROMReadLong();
  rasterHz               = EEPROMReadLong();
  lcdBacklightBrightness = EEPROMReadInt();
  squelchlevel           = EEPROMReadInt();
  mode                   = EEPROMReadByte();
  lastSelectedMemory     = EEPROMReadByte();
}

void writeGlobalSettings() {
  eeprompointer = 0;
  writeMagicNumber();                      //  2 bytes

  EEPROMWriteLong(tcxoRefHz);              //  4 bytes
  EEPROMWriteLong(rasterHz);               //  4 bytes
  EEPROMWriteInt(lcdBacklightBrightness);  //  2 bytes
  EEPROMWriteInt(squelchlevel);            //  2 bytes
  EEPROMWriteByte(mode);                   //  1 byte
  EEPROMWriteByte(lastSelectedMemory);     //  1 byte
                                           // -------
                                           // 16 bytes total
}

// Reads memory settings to EEPROM
void readMemory(int memory) {
  if (memory < 0 || memory > VFO_MEMORY_LOCATION) { return; }
  
  eeprompointer = 100 + memory * 10;
  if (!checkMagicNumber()) { defaultsMemory(memory); return; } //  2 bytes

  rxFreqHz               = EEPROMReadLong();                   //  4 bytes
  subAudioIndex          = EEPROMReadInt();                    //  2 bytes
  repeaterShiftIndex     = EEPROMReadInt();                    //  2 bytes
                                                               // --------
                                                               // 10 bytes total

  Serial.print("Read settings from EEPROM memory ");
  Serial.print(memory);
  Serial.print(", pointer:");
  Serial.println(eeprompointer);
}

void defaultsMemory(int memory) {
  switch (memory) {
    case 0: rxFreqHz  = 1298375000;  // PI6NOS
            subAudioIndex     = -1;  // -1 is no audio. See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 1: rxFreqHz  = 1298200000;  // PI6HGL
            subAudioIndex      = 8;  // 88.5 Hz, See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 2: rxFreqHz  = 1298625000;  // PI6RTD
            subAudioIndex      = 8;  // 88.5 Hz, See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 3: rxFreqHz  = 1298575000;  // PI6ASN
            subAudioIndex     = -1;  // -1 is no audio. See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 4: rxFreqHz  = 1298500000;  // PI6AMF
            subAudioIndex      = 8;  // 88.5 Hz, See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 5: rxFreqHz  = 1298400000;  // PI6EHN
            subAudioIndex     = -1;  // -1 is no audio. See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 6: rxFreqHz  = 1298425000;  // PI6NOG
            subAudioIndex      = 6;  // 82.5 Hz, See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    case 7: rxFreqHz  = 1242530000;  // PI6SHB
            subAudioIndex      = 2;  // 71.9 Hz, See subaudio.ino.
            repeaterShiftIndex = 4;  // +28 MHz, see PLL.ino
            break;
    case 8: rxFreqHz  = 1298700000;  // PI6HVN
            subAudioIndex     = -1;  // -1 is no audio. See subaudio.ino.
            repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
            break;
    default: rxFreqHz  = 1298000000;  // Simplex
             subAudioIndex     = -1;  // -1 is no audio. See subaudio.ino.
             repeaterShiftIndex = 0;  // -28 MHz, see PLL.ino
             break;
  }
}

// Writes memory settings to EEPROM
void writeMemory(int memory) {
  if (memory < 0 || memory > VFO_MEMORY_LOCATION) { return; }

  eeprompointer = 100 + memory * 10;
  writeMagicNumber();                 // 2 bytes

  EEPROMWriteLong(rxFreqHz);          // 4 bytes
  EEPROMWriteInt(subAudioIndex);      // 2 bytes
  EEPROMWriteInt(repeaterShiftIndex); // 2 bytes
  
  Serial.print("Wrote settings to EEPROM memory ");
  Serial.print(memory);
  Serial.print(", pointer:");
  Serial.println(eeprompointer);
}

boolean checkMagicNumber() {
  return magicnumber == EEPROMReadInt();
}

void writeMagicNumber() {
  EEPROMWriteInt(magicnumber);
}
 
void EEPROMWriteLong(long p_value) {
  for (byte i=0; i < 4; i++) {
    EEPROMWriteByte((byte) (p_value >> i * 8));
  }
}

long EEPROMReadLong() {
  long result = 0;
  for (byte i=0; i < 4; i++) {
    result += long(EEPROMReadByte()) << i * 8;
  }
  return result;
}

void EEPROMWriteInt(int p_value) {
  for (byte i=0; i < 2; i++) {
    EEPROMWriteByte((byte) (p_value >> i * 8));
  }
}

unsigned int EEPROMReadInt() {
  int result = 0;
  for (byte i=0; i < 2; i++) {
    result += int(EEPROMReadByte()) << i * 8;
  }
  return result;
}

void EEPROMWriteByte(byte value) {
  if (eeprompointer > EEPROM.length()) {
    Serial.println("Write EEPROM range prohibited.");
    return;
  }
  
  EEPROM.update(eeprompointer, value);
  eeprompointer++;  
}

byte EEPROMReadByte() {
  if (eeprompointer > EEPROM.length()) {
    Serial.println("Read EEPROM range prohibited.");
    return 0;
  }

  byte b = EEPROM.read(eeprompointer);
  eeprompointer++;
  return b;
}

