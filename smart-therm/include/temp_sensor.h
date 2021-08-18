#include <Arduino.h> 
#include "Wire.h"


class TempSensor {
   private:
    uint8_t _address;

   public:
    explicit TempSensor(uint8_t address);

    float tempC;
    float humidity;

    int startUpdate(const std::function<void()>& callback);

};
