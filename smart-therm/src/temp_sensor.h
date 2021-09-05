//
// Created by ambru on 2021. 09. 04..
//

#ifndef SMART_THERM_TEMP_SENSOR_H
#define SMART_THERM_TEMP_SENSOR_H

#include <Arduino.h>

#define HTU21D_ADDRESS              0x40
#define TRIGGER_TEMP_MEASURE_NOHOLD 0xF3
#define TRIGGER_HUMD_MEASURE_NOHOLD 0xF5
#define ERROR_I2C_TIMEOUT           998
#define ERROR_BAD_CRC               999
#define SHIFTED_DIVISOR 0x988000

class TempSensor {
public:
    TempSensor();

    void begin(std::function<void(float temp)> _onResult);

    void readTemperature();
    void readHumidity();

    float temp;

    void onInterval();

private:
    short attemptCount = 0;

    std::function<void(float temp)> onResult;
    bool waitingForTempResult = false;
    bool waitingForHumidityResult = false;

    void readRawValue(bool readTemp);

    static byte checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor);

    void processRawTemp(uint16_t);
};


#endif //SMART_THERM_TEMP_SENSOR_H
