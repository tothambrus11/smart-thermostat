#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <temp_sensor.h>

#include "SSD1306Wire.h"  // legacy include: `#include "SSD1306.h"`
#include "button.h"
#include "images.h"
#include "interval.h"
#include "pages.h"
#include "stringify.h"
#include "timeout.h"

SSD1306Wire display(0x3c, D2, D1);

SHT30 tempSensor(0x45);

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
    int menuPosInt;

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

            menuPosInt = (int)scrollPos % 3;
            if(menuPosInt < 0){
                menuPosInt = 3+menuPosInt;
            }

            display.drawVerticalLine(0,menuPosInt*16,16);
            break;

        default:

            break;
    };

    display.display();
}

bool aState;
bool aLastState;

#define A D5
#define B D6

void setup() {
    Serial.begin(112500);

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
        if(page == HOME){
            page = MAIN_MENU;
        } else {
            page = HOME;
        }
        redraw();
    });

    //while (sht30.get() != 0)
    //    ;
    //currentTemperature = //sht30.cTemp;

    temperatureUpdateInterval.init(1000, []() {
        tempSensor.startUpdate([]() {
            currentTemperature = tempSensor.tempC;
            digitalWrite(D8, currentTemperature < targetTemperature);

            redraw();
        });
    });

    pinMode(D8, OUTPUT);

    Serial.println("Hello world!");
}

int counter = 0;

void loop() {
    Interval::checkAll();
    Button::checkAll();

    aState = digitalRead(A);

    if (aState != aLastState) {
        bool scrolledDown = digitalRead(B) != aState;

        switch(page){
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

        Serial.println(targetTemperature);
    }
    aLastState = aState;
}
