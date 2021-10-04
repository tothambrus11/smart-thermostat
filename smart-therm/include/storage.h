#pragma once

#include <Arduino.h>  // for type definitions
#include <EEPROM.h>   // We need this library
#include "TempInterval.h"

extern const unsigned int CORRUPTION_CHECK_VALUE;
const unsigned short WIFI_SSID_MAX_LENGTH = 20;
const unsigned short WIFI_PWD_MAX_LENGTH = 20;
const unsigned short LOGIN_USERNAME_MAX_LENGTH = 16;
const unsigned short LOGIN_PASSWORD_MAX_LENGTH = 16;
extern std::vector<TempInterval> tempIntervals;
extern bool shouldSave;
extern unsigned long lastSavedAt;

template<class T>
static unsigned int EEPROM_writeAnything(int ee, const T &value);

template<class T>
static unsigned int EEPROM_readAnything(int ee, T &value);

struct WifiCredentials {
    char ssid[WIFI_SSID_MAX_LENGTH];
    char password[WIFI_PWD_MAX_LENGTH];
    unsigned short ssidLength;
    unsigned short passwordLength;
};

struct AdminLoginCredentials {
    char username[LOGIN_USERNAME_MAX_LENGTH];
    char password[LOGIN_PASSWORD_MAX_LENGTH];
    unsigned short usernameLength;
    unsigned short passwordLength;
};

const int MAX_INTERVAL_COUNT = 20;

struct StoredData {
    WifiCredentials wifiAP{};
    WifiCredentials connectedWifi{};
    AdminLoginCredentials loginCredentials{};

    unsigned int timezoneOffset{}; // minutes
    unsigned int dstOffset{}; // daylight saving time minutes

    float normalTemp{};

    std::size_t intervalCount{};
    TempInterval intervals[MAX_INTERVAL_COUNT];

    bool forcedNightMode = false;
    bool forcedDayMode = false;

    unsigned int corruptionCheck{};
};

extern StoredData storedData;

void readData();

void saveData();

void clearData();

void checkDataCorruption();

void loadIntervalsInRAM();

void saveFromRAM();

void removeInterval(size_t order);

void initMyTimesInRAM();

void saveDataSometimes();