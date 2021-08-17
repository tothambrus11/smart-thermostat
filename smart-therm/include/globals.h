#include "SSD1306Wire.h"
#include "temp_sensor.h"
#include "temp_regulator.h"
#include "ESPAsyncWebServer.h"

extern SSD1306Wire display;
extern TempSensor tempSensor;
extern TempRegulator tempRegulator;


// config variables
extern const char serverCert[] PROGMEM;

extern const char serverKey[] PROGMEM;

extern const char *ssid;
extern const char *password;
extern const char *dname;
extern AsyncWebServer server;