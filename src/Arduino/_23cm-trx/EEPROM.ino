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

const unsigned int magicnumber = 32236;
unsigned int eeprompointer = 0;
unsigned int eepromcrc     = 0;

// Reads all settings from EEPROM
void readAllFromEEPROM() {
  eeprompointer = 0;
  unsigned int checkMagic = EEPROMReadInt();
  if (checkMagic != magicnumber) {
    Serial.println("EEPROM Magic mismatch, not reading config.");
    return;
  }

  tcxoRefHz              = EEPROMReadLong();
  rasterHz               = EEPROMReadLong();
  lcdBacklightBrightness = EEPROMReadByte();
  squelchlevel           = EEPROMReadByte();
  rxFreqHz               = EEPROMReadLong();
}

// Writes all settings to EEPROM
void writeAllToEEPROM() {
  eeprompointer = 0;
  EEPROMWriteInt(magicnumber);

  EEPROMWriteLong(tcxoRefHz);
  EEPROMWriteLong(rasterHz);
  EEPROMWriteByte(lcdBacklightBrightness);
  EEPROMWriteByte(squelchlevel);
  EEPROMWriteLong(rxFreqHz);
}
 
void EEPROMWriteLong(long p_value) {
  for (byte i=0; i < 4; i++) {
    EEPROMWriteByte((byte) (p_value >> i * 8));
  }
}

unsigned int EEPROMReadLong() {
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

