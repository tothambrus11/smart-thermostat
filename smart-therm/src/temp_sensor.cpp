#include "temp_sensor.h"

TempSensor::TempSensor(uint8_t address) {
    Wire.begin();
    _address = address;
}

int TempSensor::startUpdate(std::function<void()> callback) {
    unsigned int data[6];

    // Start I2C Transmission
    Wire.beginTransmission(_address);
    // Send measurement command
    Wire.write(0x2C);
    Wire.write(0x06);
    // Stop I2C transmission
    if (Wire.endTransmission() != 0)
        return 1;

    // Request 6 bytes of data
    Wire.requestFrom(_address, (uint8_t)6);

    // Read 6 bytes of data
    // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
    for (int i = 0; i < 6; i++) {
        data[i] = Wire.read();
    };

    // Convert the data
    tempC = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
    humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

    callback();
    return 0;
}
