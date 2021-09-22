#include <Arduino.h>  // for type definitions
#include <EEPROM.h>   // We need this library
#include <globals.h>
#include "storage.h"
#include "temp_interval_functions.h"

const unsigned int CORRUPTION_CHECK_VALUE = 12345;

std::vector<TempInterval> tempIntervals;

StoredData storedData;
bool shouldSave = false;
unsigned long lastSavedAt = 0;

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

void readData() {
    EEPROM_readAnything<StoredData>(0, storedData);
    loadIntervalsInRAM();
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

    ssid = "";
    storedData.connectedWifi.ssidLength = ssid.length();
    ssid.toCharArray(storedData.connectedWifi.ssid, ssid.length() + 1);

    pass = "";
    storedData.connectedWifi.passwordLength = pass.length();
    pass.toCharArray(storedData.connectedWifi.password, pass.length() + 1);

    storedData.timezoneOffset = 0;
    storedData.dstOffset = 0;

    storedData.normalTemp = 23.5;

    String uname = "admin";
    storedData.loginCredentials.usernameLength = uname.length();
    uname.toCharArray(storedData.loginCredentials.username, uname.length() + 1);

    pass = "admin";
    storedData.loginCredentials.passwordLength = pass.length();
    pass.toCharArray(storedData.loginCredentials.password, pass.length() + 1);

    setInitialIntervals();
    saveFromRAM();
    saveData();
}

void initMyTimesInRAM() {
    for (auto &item : tempIntervals) {
        item.startTime.init(item.startHour, item.startMinute);
        item.endTime.init(item.endHour, item.endMinute);
    }
}

void saveFromRAM() {
    storedData.intervalCount = tempIntervals.size();
    for (size_t i = 0; i < tempIntervals.size(); ++i) {
        storedData.intervals[i] = tempIntervals[i];
    }
}

void loadIntervalsInRAM() {
    tempIntervals.clear();
    for (size_t i = 0; i < storedData.intervalCount; ++i) {
        tempIntervals.push_back(storedData.intervals[i]);
    }
    initMyTimesInRAM();
}

void checkDataCorruption() {
    if (storedData.corruptionCheck != CORRUPTION_CHECK_VALUE) {
        Serial.println("Corruption check failed. Clearing storage...");
        Serial.println(storedData.corruptionCheck);
        clearData();
    }
}

void removeInterval(size_t order) {
    for (size_t i = 0; i < tempIntervals.size(); i++) {
        if (tempIntervals[i].order == order) {
            tempIntervals.erase(tempIntervals.begin() + i);
            break;
        }
    } // todo no search

    for (TempInterval &interval : tempIntervals) {
        if (interval.order > order) {
            interval.order--;
        }
    }
    saveFromRAM();
    saveData();
    checkAndActivateIntervals();
}

void saveDataSometimes() {
    if (shouldSave && millis() - lastSavedAt >= 2000) {
        saveData();
        lastSavedAt = millis();
    }
}