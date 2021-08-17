#include "ipupdate.h"

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "config.h"

bool updateIp() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }

    WiFiClient client;
    HTTPClient http;

    String serverPath = "http://johetajava.hu/ip-update/update.php?password=" + String(ipUpdatePassword) + "&name=" + String(ipUpdateIdName);

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        http.end();
        return true;
    } else {
        Serial.print("Error during IP update; Error code: ");
        Serial.println(httpResponseCode);
        http.end();
        return false;
    }
}