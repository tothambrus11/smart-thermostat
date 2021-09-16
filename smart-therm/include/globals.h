#include "SSD1306Wire.h"
#include "temp_regulator.h"
#include "ESPAsyncWebServer.h"
#include "pages.h"
#include "SparkFunHTU21D.h"

extern SSD1306Wire display;
extern TempRegulator tempRegulator;
extern AsyncWebServer server;
extern AsyncWebServer localServer;
extern float tempC;
extern Page page;
extern bool isNightMode;
extern float scrollPos;
extern String drawnMessage;
extern AsyncEventSource events;