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
#include "timeout.h"
#include "routes/setup_routes.h"
#include "storage.h"
#include "drawer.h"
#include "local_server/index.h"
#include "temp_sensor.h"
#include "ESPDateTime.h"
#include "temp_interval_functions.h"

#define PIN_NIGHT_MODE_BTN D4
#define PIN_ENCODER_A D5
#define PIN_ENCODER_B D6
#define PIN_ENCODER_PUSH D7
#define PIN_SDA D2
#define PIN_SCL D1
#define PIN_RELAY D8

SSD1306Wire display(0x3c, PIN_SDA, PIN_SCL);
TempRegulator tempRegulator(PIN_RELAY);
Encoder encoder(PIN_ENCODER_A, PIN_ENCODER_B);
TempSensor tempSensor;

Button okButton(PIN_ENCODER_PUSH);
Button nightModeButton(PIN_NIGHT_MODE_BTN);

Interval temperatureUpdateInterval;
Interval temperatureReadInterval;

bool isNightMode = false;
Page page = HOME;
String drawnMessage = "";

float scrollPos = 0;
float tempC = 0;

AsyncWebServer server(3001);
AsyncWebSocket ws("/ws");            // access at ws://[esp ip]/ws
AsyncEventSource events("/events");  // event source (Server-Sent events)

void setupPins();

void setup() {
    Serial.begin(9600);
    EEPROM.begin(4096);
    readData();
    checkDataCorruption();

    display.init();
    // display.flipScreenVertically();
    display.setContrast(255);

    setupPins();

    drawMessage("Starting...");

    // SETTING UP SENSORS ==========================================

    okButton.init();
    nightModeButton.init();

    WiFi.mode(WIFI_AP_STA);

    // SETTING UP WIFI =============================================

    WiFi.softAP(String(storedData.wifiAP.ssid).substring(0, storedData.wifiAP.ssidLength),
                String(storedData.wifiAP.password).substring(0, storedData.wifiAP.passwordLength));

    WiFi.begin(String(storedData.connectedWifi.ssid).substring(0, storedData.connectedWifi.ssidLength),
               String(storedData.connectedWifi.password).substring(0, storedData.connectedWifi.passwordLength));

    drawMessage("Connecting to wifi...");

    WiFi.waitForConnectResult(10000);
    if (WiFi.isConnected()) {
        Serial.println("Connected");
    } else {
        Serial.println("WIFI connection failed");
        drawMessage("WIFI connection\n failed");
        delay(3000);// todo set normal temp while waiting for reconnection
    }

    DateTime.setTimeZone("CET-1CEST,M3.5.0,M10.5.0/3");
    DateTime.begin();
    if (!DateTime.isTimeValid()) {
        drawMessage("Failed to get time.");
        Serial.println("Failed to get time.");// todo set normal temp while waiting for reconnection
    }

    page = HOME;

    server.addHandler(&events);        // attach AsyncEventSource

    setupRoutes();
    server.begin();

    setupLocalServer();
    updateIp();

    // BUTTON HANDLERS
    nightModeButton.setOnPressListener([]() {
        changeNightMode();
        checkAndActivateIntervals();
        redraw();
    });

    okButton.setOnPressListener([]() {
        if (page == HOME) page = MAIN_MENU;
        else page = HOME;

        redraw();
    });

    tempRegulator.setTargetTemp(27.5);

    events.onConnect([](AsyncEventSourceClient *client) {
        client->send(String(tempSensor.temp).c_str(), "temperature", millis());
        client->send(String(tempRegulator.getTargetTemp()).c_str(), "target_temperature", millis());
    });

    encoder.setOnEventListener([](bool scrolledDown) {
        switch (page) {
            case HOME:
                // todo change current interval's temperature
                tempIntervals[0]->temperature += scrolledDown ? .1f : -.1f;
                checkAndActivateIntervals();

                break;
            case MAIN_MENU:
                scrollPos += scrolledDown ? 1 : -1;
                break;
            default:
                break;
        }
        redraw();
    });

    tempSensor.begin([](float temp) {
        tempC = temp;
        tempRegulator.regulateTemp();

        if (page == HOME) redraw();
        events.send(String(tempC).c_str(), "temperature", millis());
    });

    temperatureUpdateInterval.init(1000, []() {
        tempSensor.readTemperature();

        checkAndActivateIntervals();
    });

    temperatureReadInterval.init(10, []() {
        tempSensor.onInterval();
    });

}

void loop() {
    Interval::checkAll();
    Button::checkAll();
    encoder.onLoop();
}

void setupPins() {
    pinMode(PIN_ENCODER_PUSH, INPUT_PULLUP);
    pinMode(PIN_NIGHT_MODE_BTN, INPUT_PULLUP);
    pinMode(PIN_ENCODER_A, INPUT_PULLUP);
    pinMode(PIN_ENCODER_B, INPUT_PULLUP);
    pinMode(PIN_RELAY, OUTPUT);
}