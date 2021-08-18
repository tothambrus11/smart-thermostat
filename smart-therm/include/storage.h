#include <Arduino.h>  // for type definitions
#include <EEPROM.h>   // We need this library

extern const unsigned int CORRUPTION_CHECK_VALUE;

template <class T>
int EEPROM_writeAnything(int ee, const T& value);

template <class T>
int EEPROM_readAnything(int ee, T& value);

struct StoredData{
    unsigned int corruptionCheck;
};

extern StoredData storedData;

void readSettings();

void saveData();

void clearData();

void checkDataCorruption();