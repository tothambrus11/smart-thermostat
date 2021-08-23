#include <temp_regulator.h>
#include "globals.h"

void TempRegulator::regulateTemp() {
    Serial.println("Temp based on tempsensor: " + String(tempSensor.tempC));
    if (goingUp) {
        if (tempSensor.tempC >= targetTempUpperLimit) {
            setRelayState(false);
            goingUp = false;
        }
    } else {
        if (tempSensor.tempC <= targetTempLowerLimit) {
            setRelayState(true);
            goingUp = true;
        }
    }
}

void TempRegulator::setTargetTemp(float &temperature) {
    targetTemp = temperature;
    targetTempUpperLimit = targetTemp + TEMP_REGULATOR_HYSTERESIS;
    targetTempLowerLimit = targetTemp - TEMP_REGULATOR_HYSTERESIS;
}

void TempRegulator::setRelayState(bool state) {
    digitalWrite(relayPin, state);
}

TempRegulator::TempRegulator(const unsigned char pin) : relayPin(pin) {
    float t = 28.5;
    setTargetTemp(t);

    goingUp = tempSensor.tempC < targetTempUpperLimit;
    setRelayState(goingUp);
}
