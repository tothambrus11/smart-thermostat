#include "SSD1306Wire.h"
#include "temp_sensor.h"
#include "temp_regulator.h"
#include "ESPAsyncWebServer.h"

extern SSD1306Wire display;
extern TempSensor tempSensor;
extern TempRegulator tempRegulator;
extern AsyncWebServer server;