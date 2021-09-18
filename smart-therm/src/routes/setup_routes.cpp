#include <storage.h>
#include <temp_interval_functions.h>
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
        checkAndActivateIntervals();
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

        auto *response = new AsyncJsonResponse(false, 7000);
        JsonObject doc = response->getRoot();

        doc["normalTemp"] = storedData.normalTemp;

        auto arr = doc.createNestedArray("items");
        for (auto &tempInterval : tempIntervals) {
            JsonObject data = arr.createNestedObject();
            Serial.println(tempInterval->type);
            data["type"] = tempInterval->type;
            data["temperature"] = tempInterval->temperature;
            data["enabled"] = tempInterval->enabled;
            data["repetitionFrequency"] = tempInterval->repetitionFrequency;
            data["daysOfWeek"] = tempInterval->daysOfWeek;
            data["startYear"] = tempInterval->startYear;
            data["startMonth"] = tempInterval->startMonth;
            data["startDay"] = tempInterval->startDay;
            data["startHour"] = tempInterval->startHour;
            data["startMinute"] = tempInterval->startMinute;
            data["endYear"] = tempInterval->endYear;
            data["endMonth"] = tempInterval->endMonth;
            data["endDay"] = tempInterval->endDay;
            data["endHour"] = tempInterval->endHour;
            data["endMinute"] = tempInterval->endMinute;
            data["order"] = tempInterval->order;
        }

        response->setLength();
        request->send(response);
    });

    server.on("/remove-interval", HTTP_GET, [](AsyncWebServerRequest *request) {
        int order = request->getParam("order")->value().toInt();
        removeInterval(order);
        request->send(200, "text/plain", OK_RESPONSE + String(order));
    });

    server.on("/reset-intervals", HTTP_GET, [](AsyncWebServerRequest *request) {
        getInitialIntervals(tempIntervals);
        saveFromRAM();
        saveData();
        request->send(200, "text/plain", OK_RESPONSE);
    });


    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", OK_RESPONSE);
        ESP.restart();
    });


    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "404 Not Found");
    });
}