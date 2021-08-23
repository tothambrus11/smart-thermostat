#include <Arduino.h>  // for type definitions
#include <EEPROM.h>   // We need this library
#include "storage.h"

const unsigned int CORRUPTION_CHECK_VALUE = 12345678;

//We create two fucntions for writing and reading data from the EEPROM
template<class T>
static unsigned int EEPROM_writeAnything(int addr, const T &value) {
    const byte *p = (const byte *) (const void *) &value;

    unsigned int i;

    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(addr++, *p++);

    return i;
}

template<class T>
static unsigned int EEPROM_readAnything(int addr, T &value) {
    byte *p = (byte *) (void *) &value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(addr++);

    return i;
}

StoredData storedData;

void readData() {
    EEPROM_readAnything<StoredData>(0, storedData);
}

void saveData() {
    EEPROM_writeAnything<StoredData>(0, storedData);
    EEPROM.commit();
}

void clearData() {
    storedData.corruptionCheck = CORRUPTION_CHECK_VALUE;
    storedData.connectedWifi.ssidLength = 0;
    storedData.connectedWifi.passwordLength = 0;

    String ssid = "ESP Thermostat";
    storedData.wifiAP.ssidLength = ssid.length();
    ssid.toCharArray(storedData.wifiAP.ssid, ssid.length() + 1);

    String pass = "z3rowG@6k";
    storedData.wifiAP.passwordLength = pass.length();
    pass.toCharArray(storedData.wifiAP.password, pass.length() + 1);

    storedData.timezoneOffset = 0;
    storedData.dstOffset = 0;

    saveData();
}

void checkDataCorruption() {
    if (storedData.corruptionCheck != CORRUPTION_CHECK_VALUE) {
        Serial.println("Corruption check");
        Serial.println(storedData.corruptionCheck);
        clearData();
    }
}