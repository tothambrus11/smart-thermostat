#include <Arduino.h>  // for type definitions
#include <EEPROM.h>   // We need this library
#ifndef STORAGE_H
#define STORAGE_H

extern const unsigned int CORRUPTION_CHECK_VALUE;
const unsigned short WIFI_SSID_MAX_LENGTH = 32;
const unsigned short WIFI_PWD_MAX_LENGTH = 32;

template <class T>
static unsigned int EEPROM_writeAnything(int ee, const T& value);

template <class T>
static unsigned int EEPROM_readAnything(int ee, T& value);

struct WifiCredentials{
    char ssid[WIFI_SSID_MAX_LENGTH];
    char password[WIFI_SSID_MAX_LENGTH];
    unsigned short ssidLength;
    unsigned short passwordLength;
};

struct StoredData{
    WifiCredentials wifiAP;
    WifiCredentials connectedWifi;
    unsigned int timezoneOffset; // minutes
    unsigned int dstOffset; // daylight saving time minutes

    unsigned int corruptionCheck;
};

extern StoredData storedData;

void readData();

void saveData();

void clearData();

void checkDataCorruption();

#endif