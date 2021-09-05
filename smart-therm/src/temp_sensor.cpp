#include "temp_sensor.h"
#include "Wire.h"

TempSensor::TempSensor() {}

void TempSensor::begin(std::function<void(float temp)> _onResult) {
    onResult = std::move(_onResult);
    Wire.begin();
}

void TempSensor::readTemperature() {
    readRawValue(TRIGGER_TEMP_MEASURE_NOHOLD);
}

void TempSensor::readHumidity() {
    readRawValue(TRIGGER_HUMD_MEASURE_NOHOLD);
}

void TempSensor::processRawTemp(uint16_t rawTemp){
    temp = rawTemp * (175.72f / 65536.0f) - 46.85f;
    onResult(temp);
}

void TempSensor::readRawValue(bool readTemp) {
    Wire.beginTransmission(HTU21D_ADDRESS);
    Wire.write(readTemp ? TRIGGER_TEMP_MEASURE_NOHOLD : TRIGGER_HUMD_MEASURE_NOHOLD); //Measure value (prefer no hold!)
    Wire.endTransmission();

    if(readTemp){
        waitingForTempResult = true;
    } else {
        waitingForHumidityResult = true;
    }
}

void TempSensor::onInterval() {
    if (waitingForTempResult) {
        bool validResult = (3 == Wire.requestFrom(HTU21D_ADDRESS, 3));

        if(validResult){
            waitingForTempResult = false;
            attemptCount = 0;
            byte msb = Wire.read();
            byte lsb = Wire.read();
            byte checksum = Wire.read();

            uint16_t rawValue = ((uint16_t) msb << 8) | (uint16_t) lsb;

            if (checkCRC(rawValue, checksum) != 0) return;

            processRawTemp(rawValue & 0xFFFC);
        } else {
            attemptCount++;
        }

        if(attemptCount >= 10){
            waitingForTempResult = false;
            attemptCount = 0;
        }
    }
}

byte TempSensor::checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor) {
    auto remainder = (uint32_t) message_from_sensor << 8;
    remainder |= check_value_from_sensor; //Add on the check value

    auto divisor = (uint32_t) SHIFTED_DIVISOR;

    for (int i = 0; i < 16; i++) {
        if (remainder & (uint32_t) 1 << (23 - i)) { remainder ^= divisor; }
        divisor >>= 1;
    }

    return (byte) remainder;
}

