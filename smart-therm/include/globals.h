#include "SSD1306Wire.h"
#include "temp_sensor.h"
#include "temp_regulator.h"
#include "ESPAsyncWebServer.h"
#include "pages.h"

extern SSD1306Wire display;
extern TempSensor tempSensor;
extern TempRegulator tempRegulator;
extern AsyncWebServer server;
extern AsyncWebServer localServer;
extern Page page;
extern bool isNightMode;
extern float targetTemperature;
extern float scrollPos;
