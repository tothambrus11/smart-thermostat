#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Wire.h>

#include "ESPAsyncWebServer.h"
#include "SSD1306Wire.h"
#include "button.h"
#include "config.h"
#include "images.h"
#include "interval.h"
#include "ipupdate.h"
#include "pages.h"
#include "stringify.h"
#include "temp_regulator.h"
#include "temp_sensor.h"
#include "timeout.h"
#include "routes/setup_routes.h"
#include "storage.h"
#include "stored_images.h"

SSD1306Wire display(0x3c, D2, D1);
TempSensor tempSensor(0x45);
TempRegulator tempRegulator;

Button okButton(D7);
Button nightModeButton(D4);

Interval temperatureUpdateInterval;

bool isNightMode = false;
Page page = HOME;

float currentTemperature;
float targetTemperature = 29.5;

float scrollPos = 0;

void redraw() {
    display.clear();
    short menuPosInt;

    switch (page) {
        case HOME:

            display.drawFastImage(108, 25, 16, 16, isNightMode ? epd_bitmap_hold : epd_bitmap_sun);

            //display.drawCircle(64 + (int)((targetTemperature-29.5)*10), 31, 16);
            if (currentTemperature < targetTemperature) {
                display.drawFastImage(20, 15, 16, 16, epd_bitmap_nyil);
            } else if (currentTemperature > targetTemperature) {
                display.drawFastImage(20, 15, 16, 16, epd_bitmap_nyil_le);
            }

            display.setTextAlignment(TEXT_ALIGN_RIGHT);
            display.setFont(ArialMT_Plain_16);
            display.drawString(82, 15, stringify(targetTemperature) + "°C");

            display.setFont(ArialMT_Plain_24);

            display.drawString(82, 32, stringify(currentTemperature) + "°C");
            break;

        case MAIN_MENU:
            display.setFont(ArialMT_Plain_16);
            display.setTextAlignment(TEXT_ALIGN_LEFT);
            for (uint8_t row = 0; row < 3; row++) {
                display.drawString(10, 16 * row, "text " + String(row));
            }

            menuPosInt = (short) scrollPos % 3;
            if (menuPosInt < 0) {
                menuPosInt += 3;
            }

            display.drawVerticalLine(0, menuPosInt * 16, 16);
            break;

        default:

            break;
    }

    display.display();
}

bool aState;
bool aLastState;

#define A D5
#define B D6

AsyncWebServer server(3001);
AsyncWebSocket ws("/ws");            // access at ws://[esp ip]/ws
AsyncEventSource events("/events");  // event source (Server-Sent events)

bool connectToWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    return WiFi.waitForConnectResult() == WL_CONNECTED;
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    //Handle body
}

void
onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
    //Handle upload
}

void onEvent(AsyncWebSocket *asyncWebSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
             size_t len) {
    //Handle WebSocket event
}

void setup() {
    Serial.begin(9600);

    display.init();

    display.flipScreenVertically();
    display.setContrast(255);

    pinMode(D4, INPUT_PULLUP);
    pinMode(D5, INPUT_PULLUP);
    pinMode(D6, INPUT_PULLUP);
    pinMode(D7, INPUT_PULLUP);

    aLastState = digitalRead(A);

    redraw();

    nightModeButton.setOnPressListener([]() {
        isNightMode = !isNightMode;
        redraw();
    });

    okButton.setOnPressListener([]() {
        if (page == HOME) {
            page = MAIN_MENU;
        } else {
            page = HOME;
        }
        redraw();

    });


    temperatureUpdateInterval.init(1000, []() {
        tempSensor.startUpdate([]() {
            currentTemperature = tempSensor.tempC;
            tempRegulator.regulateTemp();

            redraw();
        });
    });

    pinMode(D8, OUTPUT);

    Serial.println("Hello world!");

    if (!connectToWifi()) {
        Serial.println("WIFI connection failed");
    } else {
        configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

        // attach AsyncWebSocket
        ws.onEvent(onEvent);
        server.addHandler(&ws);

        // attach AsyncEventSource
        server.addHandler(&events);

        setupRoutes();

        // Catch-All Handlers
        // Any request that can not find a Handler that canHandle it
        // ends in the callbacks below.
        server.onFileUpload(onUpload);
        server.onRequestBody(onBody);

        server.begin();
        updateIp();

        Serial.println("Server is ready");
    }

}

int counter = 0;

void loop() {
    Interval::checkAll();
    Button::checkAll();

    aState = digitalRead(A);

    if (aState != aLastState) {
        bool scrolledDown = digitalRead(B) != aState;

        switch (page) {
            case HOME:
                targetTemperature += scrolledDown ? .05 : -.05;
                break;
            case MAIN_MENU:
                scrollPos += scrolledDown ? .5 : -.5;
                break;
            default:
                break;
        }
        redraw();
    }
    aLastState = aState;

}

