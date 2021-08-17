#include "globals.h"

void route404() {
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "404 Not Found");
    });
}