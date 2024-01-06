#include<EEPROM.h>

/* Defining data storage addresses
   As we know size of int is 4 Bytes,
    If we store int at index 0 then the next EEPROM address can be anything (4,5..10 etc.) after 4 byte address i.e. (0-3)
  You can use this statement to find size of int type data Serial.println(sizeof(int))
*/
int eepromAddr1 = 0, eepromAddr2 = 40, eepromAddr3 = 80;

// int and string data which we store in EEPROM of Arduino Uno or Arduino Nano etc.
int eepromSize = 128; //can be up to 4Kb, but we don't need that much, let's start with 128B.


void loadEEPROM() {
  EEPROM.begin(eepromSize);
  delay(500);
  eeprom_net = EEPROM.readString(eepromAddr1);
  eeprom_pass = EEPROM.readString(eepromAddr2);
  if(eeprom_net==""){
    eeprom_net="******";
  }
  if(eeprom_pass==""){
    eeprom_pass="*******";
  }
//  eeprom_user = EEPROM.readString(eepromAddr3);
  Serial.println("Read from EEPROM");
  Serial.print("net:");
  Serial.println(eeprom_net);
  Serial.print("pass:");
  Serial.println(eeprom_pass);
  Serial.print("user:\"");
  Serial.print(eeprom_user);
  Serial.println("\"");
}
