#include "ESP8266WiFi.h"
#include "ESP8266mDNS.h"
#include "Wire.h"
#include "encoder.h"
#include "ESPAsyncWebServer.h"
#include "SSD1306Wire.h"
#include "button.h"
#include "config.h"
#include "images.h"
#include "interval.h"
#include "ipupdate.h"
#include "pages.h"
#include "temp_regulator.h"
#include "temp_sensor.h"
#include "timeout.h"
#include "routes/setup_routes.h"
#include "storage.h"
#include "drawer.h"
#include "local_server/index.h"

#define PIN_NIGHT_MODE_BTN D4
#define PIN_ENCODER_A D5
#define PIN_ENCODER_B D6
#define PIN_ENCODER_PUSH D7
#define PIN_SDA D2
#define PIN_SCL D1
#define PIN_RELAY D8

SSD1306Wire display(0x3c, PIN_SDA, PIN_SCL);
TempSensor tempSensor(0x45);
TempRegulator tempRegulator(PIN_RELAY);
Encoder encoder(PIN_ENCODER_A, PIN_ENCODER_B);

Button okButton(PIN_ENCODER_PUSH);
Button nightModeButton(PIN_NIGHT_MODE_BTN);

Interval temperatureUpdateInterval;

bool isNightMode = false;
Page page = HOME;

float targetTemperature = 29.5;
float scrollPos = 0;

AsyncWebServer server(3001);
AsyncWebSocket ws("/ws");            // access at ws://[esp ip]/ws
AsyncEventSource events("/events");  // event source (Server-Sent events)

bool connectToWifi() {
    WiFi.begin(ssid, password);
    return WiFi.waitForConnectResult(10000) == WL_CONNECTED;
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
    pinMode(PIN_ENCODER_A, INPUT_PULLUP);
    pinMode(PIN_ENCODER_B, INPUT_PULLUP);
    pinMode(D7, INPUT_PULLUP);

    EEPROM.begin(4096);


    redraw();

    nightModeButton.setOnPressListener([]() {
        isNightMode = !isNightMode;
        redraw();
    });

    okButton.setOnPressListener([]() {
        Serial.println("ok pressed");
        if (page == HOME) {
            page = MAIN_MENU;
        } else {
            page = HOME;
        }
        redraw();

    });

    okButton.init();
    nightModeButton.init();


    temperatureUpdateInterval.init(1000, []() {
        tempSensor.startUpdate([]() {
            tempRegulator.regulateTemp();

            redraw();
            events.send(String(tempSensor.tempC).c_str(), "temperature", millis());
        });
    });


    encoder.setOnEventListener([](bool scrolledDown) {
        Serial.println(targetTemperature);
        switch (page) {
            case HOME:
                targetTemperature += scrolledDown ? .1 : -.1;
                break;
            case MAIN_MENU:
                scrollPos += scrolledDown ? 1 : -1;
                break;
            default:
                break;

        }
        redraw();
    });

    pinMode(D8, OUTPUT);

    Serial.println("Hello world!");

    WiFi.mode(WIFI_AP_STA);

    WiFi.softAP(apSSID, apPassword);
    if (!connectToWifi()) {
        Serial.println("WIFI connection failed");
    } else {
        configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    }


    ws.onEvent(onEvent);        // attach AsyncWebSocket
    server.addHandler(&ws);
    server.addHandler(&events);        // attach AsyncEventSource

    server.onFileUpload(onUpload);
    server.onRequestBody(onBody);

    setupRoutes();
    setupLocalServer();

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.begin();
    updateIp();

    Serial.println("Server is ready");

    readData();
    checkDataCorruption();

    readData();
    checkDataCorruption();

}

void loop() {
    Interval::checkAll();
    Button::checkAll();
    encoder.onLoop();
}

