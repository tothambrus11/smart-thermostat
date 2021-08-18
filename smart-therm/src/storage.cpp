#include <Arduino.h>  // for type definitions
#include <EEPROM.h>   // We need this library
#include "storage.h"

const unsigned int CORRUPTION_CHECK_VALUE = 12345678;

//We create two fucntions for writing and reading data from the EEPROM
template <class T>
int EEPROM_writeAnything(int ee, const T& value) {
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

template <class T>
int EEPROM_readAnything(int ee, T& value) {
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}

StoredData storedData;

void readSettings() {
    EEPROM_readAnything(0, storedData);
}

void saveData() {
    EEPROM_writeAnything(0, storedData);
    EEPROM.commit();
}

void clearData() {
    storedData.corruptionCheck = CORRUPTION_CHECK_VALUE;
    saveData();
}

void checkDataCorruption(){
    if(storedData.corruptionCheck != CORRUPTION_CHECK_VALUE){
        clearData();
    }
}