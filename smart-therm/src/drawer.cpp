#include "drawer.h"
#include "stored_images.h"
#include "stringify.h"
#include "globals.h"
#include "pages.h"

void redraw() {
    display.clear();
    short menuPosInt;

    switch (page) {
        case HOME:

            display.drawFastImage(108, 25, 16, 16, isNightMode ? epd_bitmap_hold : epd_bitmap_sun);

            //display.drawCircle(64 + (int)((targetTemperature-29.5)*10), 31, 16);
            if (tempSensor.tempC < targetTemperature) {
                display.drawFastImage(20, 15, 16, 16, epd_bitmap_nyil);
            } else if (tempSensor.tempC > targetTemperature) {
                display.drawFastImage(20, 15, 16, 16, epd_bitmap_nyil_le);
            }

            display.setTextAlignment(TEXT_ALIGN_RIGHT);
            display.setFont(ArialMT_Plain_16);
            display.drawString(82, 15, stringify(targetTemperature) + "°C");

            display.setFont(ArialMT_Plain_24);

            display.drawString(82, 32, stringify(tempSensor.tempC) + "°C");
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