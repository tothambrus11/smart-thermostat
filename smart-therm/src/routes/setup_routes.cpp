#include <storage.h>
#include <temp_interval_functions.h>
#include <interval_type.h>
#include <repetition_frequency.h>
#include "globals.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "drawer.h"

const String OK_RESPONSE = "ok";

bool checkAuthentication(AsyncWebServerRequest *req) {
    if (req->hasParam("username", true) && req->hasParam("password", true) &&
        String(storedData.loginCredentials.username).substring(0, storedData.loginCredentials.usernameLength) ==
        req->getParam("username", true)->value() &&
        String(storedData.loginCredentials.password).substring(0, storedData.loginCredentials.passwordLength) ==
        req->getParam("password", true)->value()) {
        return true;
    }
    req->send(401);
    return false;

}

void setupRoutes() {

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.on("/get-is-heating", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        request->send(200, "text/plain", tempRegulator.goingUp ? "1" : "0");
    });

    server.on("/get-temp", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        request->send(200, "text/plain", String(tempC));
    });

    server.on("/get-target-temp", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        request->send(200, "text/plain", String(tempRegulator.getTargetTemp()));
    });

    server.on("/get-normal-temp", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        request->send(200, "text/plain", String(storedData.normalTemp));
    });

    server.on("/get-normal-temp", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        request->send(200, "text/plain", String(storedData.normalTemp));
    });

    server.on("/set-normal-temp", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        storedData.normalTemp = request->getParam("temp")->value().toFloat();
        storedData.normalTemp = max(min(50.0f, storedData.normalTemp), 0.0f);
        shouldSave = true;
        checkAndActivateIntervals();
        shouldRedraw = true;
        request->send(200, "text/plain", OK_RESPONSE);
    });

    server.on("/reset", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        clearData();
        request->send(200, "text/plain", OK_RESPONSE);
    });
    server.on("/get-size", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        request->send(200, "text/plain", String(storedData.intervalCount));
    });

    server.on("/modify-property", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
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


    server.on("/get-intervals", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

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

    server.on("/get-network-data", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        auto *response = new AsyncJsonResponse(false, 256);
        JsonObject doc = response->getRoot();

        doc["wifiSSID"] = String(storedData.connectedWifi.ssid).substring(0, storedData.connectedWifi.ssidLength);
        doc["localIP"] = WiFi.localIP().toString();
        doc["mac"] = WiFi.macAddress();
        doc["port"] = SERVER_PORT;

        response->setLength();
        request->send(response);
    });

    server.on("/get-ap-credentials", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        auto *response = new AsyncJsonResponse(false, 256);
        JsonObject doc = response->getRoot();

        doc["ssid"] = String(storedData.wifiAP.ssid).substring(0,storedData.wifiAP.ssidLength);
        doc["password"] = String(storedData.wifiAP.password).substring(0,storedData.wifiAP.passwordLength);

        response->setLength();
        request->send(response);
    });

    server.on("/set-wifi-credentials", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        String ssid = request->getParam("wifiSSID", true)->value();
        String password = request->getParam("wifiPassword", true)->value();

        ssid.toCharArray(storedData.connectedWifi.ssid, ssid.length() + 1);
        password.toCharArray(storedData.connectedWifi.password, password.length() + 1);

        storedData.connectedWifi.ssidLength = ssid.length();
        storedData.connectedWifi.passwordLength = password.length();

        shouldSave = true;
        request->send(200,"application/json", "null");
    });

    server.on("/get-active-interval", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        if (activeIntervalOrder != -1) {
            auto *response = new AsyncJsonResponse(false, 7000);
            auto &tempInterval = tempIntervals[activeIntervalOrder];
            JsonObject doc = response->getRoot();
            doc["type"] = tempInterval.type;
            doc["temperature"] = tempInterval.temperature;
            doc["enabled"] = tempInterval.enabled;
            doc["repetitionFrequency"] = tempInterval.repetitionFrequency;
            doc["daysOfWeek"] = tempInterval.daysOfWeek;
            doc["startYear"] = tempInterval.startYear;
            doc["startMonth"] = tempInterval.startMonth;
            doc["startDay"] = tempInterval.startDay;
            doc["startHour"] = tempInterval.startHour;
            doc["startMinute"] = tempInterval.startMinute;
            doc["endYear"] = tempInterval.endYear;
            doc["endMonth"] = tempInterval.endMonth;
            doc["endDay"] = tempInterval.endDay;
            doc["endHour"] = tempInterval.endHour;
            doc["endMinute"] = tempInterval.endMinute;
            doc["order"] = tempInterval.order;
            response->setLength();
            request->send(response);
        } else {
            request->send(200, "application/json", "null");
        }
    });

    server.on("/change-interval-order", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;
        int order1 = request->getParam("order1")->value().toInt();
        int order2 = request->getParam("order2")->value().toInt();

        if (order1 < 0 || order2 < 0 || order1 >= tempIntervals.size() || order2 >= tempIntervals.size() ||
            order1 == order2) {
            request->send(400, "text/plain", "");
            return;
        }

        std::swap(tempIntervals[order1], tempIntervals[order2]);
        std::swap(tempIntervals[order1].order, tempIntervals[order2].order);
        saveFromRAM();
        checkAndActivateIntervals();
        shouldSave = true;
        shouldRedraw = true;
        request->send(200, "text/plain", OK_RESPONSE + String(OK_RESPONSE));
    });

    server.on("/remove-interval", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        int order = request->getParam("order")->value().toInt();
        if (order >= tempIntervals.size() || order < 0 || tempIntervals[order].type == IntervalType::NIGHT) {
            request->send(400);
            return;
        }
        removeInterval(order);
        request->send(200, "text/plain", OK_RESPONSE + String(order));
    });

    server.on("/reset-intervals", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        setInitialIntervals();
        saveFromRAM();
        saveData();
        request->send(200, "text/plain", OK_RESPONSE);
    });


    server.on("/restart", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        request->send(200, "text/plain", OK_RESPONSE);
        ESP.restart();
    });

    server.on("/new-interval", HTTP_ANY, [](AsyncWebServerRequest *request) {
        if (!checkAuthentication(request)) return;

        tempIntervals.push_back({
                                        IntervalType::CUSTOM,
                                        23,
                                        false,
                                        RepetitionFrequency::DAILY,
                                        0,
                                        0,
                                        0,
                                        0,
                                        10,
                                        0,
                                        0,
                                        0,
                                        0,
                                        14,
                                        0,
                                        static_cast<int>(tempIntervals.size())
                                });
        saveFromRAM();
        shouldSave = true;
        // Redraw and check is not needed since the interval is disabled by default.
        request->send(200, "text/plain", OK_RESPONSE);
    });


    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "404 Not Found");
    });
}