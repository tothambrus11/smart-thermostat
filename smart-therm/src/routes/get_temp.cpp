#include "globals.h"
#include "routes/route_get_temp.h"

void routeGetTemp(){
    server.on("/get-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(tempSensor.tempC));
    });
}