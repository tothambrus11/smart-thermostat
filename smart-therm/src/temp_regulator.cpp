#include <temp_regulator.h>
#include "globals.h"

void TempRegulator::regulateTemp() {
    Serial.println("Measured temperature: " + String(tempC));
    if (goingUp) {
        if (tempC >= targetTempUpperLimit) {
            setRelayState(false);
            goingUp = false;
        }
    } else {
        if (tempC <= targetTempLowerLimit) {
            setRelayState(true);
            goingUp = true;
        }
    }
}

void TempRegulator::setTargetTemp(float temperature) {
    targetTemp = temperature;
    targetTempUpperLimit = targetTemp + TEMP_REGULATOR_HYSTERESIS;
    targetTempLowerLimit = targetTemp - TEMP_REGULATOR_HYSTERESIS;

    goingUp = tempC < targetTempUpperLimit;
    setRelayState(goingUp);
}

void TempRegulator::setRelayState(bool state) {
    digitalWrite(relayPin, state);
}

TempRegulator::TempRegulator(const unsigned char pin) : relayPin(pin) {

}

float TempRegulator::getTargetTemp() {
    return targetTemp;
}

