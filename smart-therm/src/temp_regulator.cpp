#include "globals.h"

void TempRegulator::regulateTemp() {
    Serial.println("Temp based on tempsensor: " + String(tempSensor.tempC));
}
