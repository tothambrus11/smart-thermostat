#include "local_server/index.h"
#include "globals.h"
#include "local_server/wifi_settings.h"

AsyncWebServer localServer(80);

void setupLocalServer(){
    routeWifiSettings();
    routeSaveWifiSettings();
    localServer.begin();
}