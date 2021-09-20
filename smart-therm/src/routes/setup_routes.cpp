#include <storage.h>
#include <temp_interval_functions.h>
#include "globals.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "drawer.h"

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
        storedData.normalTemp = max(min(50.0f, storedData.normalTemp), 0.0f);
        shouldSave = true;
        checkAndActivateIntervals();
        shouldRedraw = true;
        request->send(200, "text/plain", OK_RESPONSE);
    });

    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
        clearData();
        request->send(200, "text/plain", OK_RESPONSE);
    });
    server.on("/get-size", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(storedData.intervalCount));
    });

    server.on("/modify-property", HTTP_GET, [](AsyncWebServerRequest *request) {
        auto param = request->getParam("param")->value();
        auto value = request->getParam("value")->value();
        size_t intervalIndex = request->getParam("index")->value().toInt();

        if (intervalIndex >= tempIntervals.size()) {
            request->send(400, "text/plain", "Invalid index");
            return;
        }

        if (param == "temperature") {
            tempIntervals[intervalIndex].temperature = value.toFloat();
        } else if (param == "daysOfWeek") {
            tempIntervals[intervalIndex].daysOfWeek = value.toInt();
        } else if (param == "enabled") {
            tempIntervals[intervalIndex].enabled = value.toInt() == 1;
        } else if (param == "repetitionFrequency") {
            tempIntervals[intervalIndex].repetitionFrequency = value.toInt();
        } else if (param == "startTime") { // format: HH:MM
            tempIntervals[intervalIndex].startHour = value.substring(0, 2).toInt();
            tempIntervals[intervalIndex].startMinute = value.substring(3, 5).toInt();
            tempIntervals[intervalIndex].startTime.init(tempIntervals[intervalIndex].startHour,
                                                        tempIntervals[intervalIndex].startMinute);
        } else if (param == "endTime") {
            tempIntervals[intervalIndex].endHour = value.substring(0, 2).toInt();
            tempIntervals[intervalIndex].endMinute = value.substring(3, 5).toInt();
            tempIntervals[intervalIndex].endTime.init(tempIntervals[intervalIndex].endHour,
                                                      tempIntervals[intervalIndex].endMinute);
        } else if (param == "startDateTime") { // YYYY-MM-ddThh:mm
            tempIntervals[intervalIndex].startYear = value.substring(0, 4).toInt();
            tempIntervals[intervalIndex].startMonth = value.substring(5, 7).toInt();
            tempIntervals[intervalIndex].startDay = value.substring(8, 10).toInt();
            tempIntervals[intervalIndex].startHour = value.substring(11, 13).toInt();
            tempIntervals[intervalIndex].startMinute = value.substring(14, 16).toInt();
            tempIntervals[intervalIndex].startTime.init(tempIntervals[intervalIndex].startHour,
                                                        tempIntervals[intervalIndex].startMinute);
        } else if (param == "endDateTime") {
            tempIntervals[intervalIndex].endYear = value.substring(0, 4).toInt();
            tempIntervals[intervalIndex].endMonth = value.substring(5, 7).toInt();
            tempIntervals[intervalIndex].endDay = value.substring(8, 10).toInt();
            tempIntervals[intervalIndex].endHour = value.substring(11, 13).toInt();
            tempIntervals[intervalIndex].endMinute = value.substring(14, 16).toInt();
            tempIntervals[intervalIndex].endTime.init(tempIntervals[intervalIndex].endHour,
                                                      tempIntervals[intervalIndex].endMinute);
        }

        checkAndActivateIntervals();

        saveFromRAM();
        shouldSave = true;
        shouldRedraw = true;

        request->send(200, "text/plain", "ok");
    });


    server.on("/get-intervals", HTTP_GET, [](AsyncWebServerRequest *request) {

        auto *response = new AsyncJsonResponse(false, 7000);
        JsonObject doc = response->getRoot();

        doc["normalTemp"] = storedData.normalTemp;

        auto arr = doc.createNestedArray("items");
        for (auto &tempInterval : tempIntervals) {
            JsonObject data = arr.createNestedObject();
            data["type"] = tempInterval.type;
            data["temperature"] = tempInterval.temperature;
            data["enabled"] = tempInterval.enabled;
            data["repetitionFrequency"] = tempInterval.repetitionFrequency;
            data["daysOfWeek"] = tempInterval.daysOfWeek;
            data["startYear"] = tempInterval.startYear;
            data["startMonth"] = tempInterval.startMonth;
            data["startDay"] = tempInterval.startDay;
            data["startHour"] = tempInterval.startHour;
            data["startMinute"] = tempInterval.startMinute;
            data["endYear"] = tempInterval.endYear;
            data["endMonth"] = tempInterval.endMonth;
            data["endDay"] = tempInterval.endDay;
            data["endHour"] = tempInterval.endHour;
            data["endMinute"] = tempInterval.endMinute;
            data["order"] = tempInterval.order;
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
        setInitialIntervals();
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