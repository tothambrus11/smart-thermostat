#include <Arduino.h> 
#include "Wire.h"


class TempSensor {
   private:
    uint8_t _address;

   public:
    TempSensor(uint8_t address);

    float tempC;
    float humidity;

    int startUpdate(std::function<void()> callback);

    void onLoop();
};
