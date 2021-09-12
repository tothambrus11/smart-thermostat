#include <storage.h>
#include "globals.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

const String OK_RESPONSE = "ok";


void setupRoutes() {

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.on("/get-is-heating", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", tempRegulator.goingUp ? "1" : "0");
    });

    server.on("/get-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(tempC));
    });

    server.on("/get-target-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(tempRegulator.getTargetTemp()));
    });

    server.on("/get-normal-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(storedData.normalTemp));
    });

    server.on("/get-normal-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(storedData.normalTemp));
    });

    server.on("/set-normal-temp", HTTP_GET, [](AsyncWebServerRequest *request) {
        storedData.normalTemp = request->getParam("temp")->value().toFloat();
        saveData();
        // todo notify tempregulator
        request->send(200, "text/plain", OK_RESPONSE); // todo optional error handling
    });

    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
        clearData();
        request->send(200, "text/plain", OK_RESPONSE);
    });
    server.on("/get-size", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(storedData.intervalCount));
    });

    server.on("/get-intervals", HTTP_GET, [](AsyncWebServerRequest *request) {

        auto * response = new AsyncJsonResponse(false, 7000);
        JsonObject doc = response->getRoot();

        doc["normalTemp"] = storedData.normalTemp;

        auto arr = doc.createNestedArray("items");
        for (std::size_t i = 0; i < storedData.intervalCount && i <= 20; ++i) {
            JsonObject data = arr.createNestedObject();
            data["type"] = storedData.intervals[i].type;
            data["temperature"] = storedData.intervals[i].temperature;
            data["enabled"] = storedData.intervals[i].enabled;
            data["repetitionFrequency"] = storedData.intervals[i].repetitionFrequency;
            data["daysOfWeek"] = storedData.intervals[i].daysOfWeek;
            data["startYear"] = storedData.intervals[i].startYear;
            data["startMonth"] = storedData.intervals[i].startMonth;
            data["startDay"] = storedData.intervals[i].startDay;
            data["startHour"] = storedData.intervals[i].startHour;
            data["startMinute"] = storedData.intervals[i].startMinute;
            data["endYear"] = storedData.intervals[i].endYear;
            data["endMonth"] = storedData.intervals[i].endMonth;
            data["endDay"] = storedData.intervals[i].endDay;
            data["endHour"] = storedData.intervals[i].endHour;
            data["endMinute"] = storedData.intervals[i].endMinute;
            data["order"] = storedData.intervals[i].order;
        }

        response->setLength();
        request->send(response);
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "404 Not Found");
    });
}