#include "globals.h"

void setupRoutes() {
    server.on("/get-is-heating", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", tempRegulator.goingUp ? "1" : "0");
    });

    server.on("/get-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(tempC));
    });

    server.on("/get-target-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(tempRegulator.getTargetTemp()));
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "404 Not Found");
    });
}