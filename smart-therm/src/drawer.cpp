#include "drawer.h"
#include "stored_images.h"
#include "stringify.h"
#include "globals.h"
#include "pages.h"
#include "temp_interval_functions.h"

void redraw() {
    display.clear();
    short menuPosInt;

    switch (page) {
        case HOME:

            display.drawFastImage(108, 25, 16, 16, actualNightMode ? epd_bitmap_hold : epd_bitmap_sun);

            if (tempRegulator.goingUp) {
                display.drawFastImage(20, 15, 16, 16, epd_bitmap_nyil);
            } else if (tempC > tempRegulator.getTargetTemp()) {
                display.drawFastImage(20, 15, 16, 16, epd_bitmap_nyil_le);
            }

            display.setTextAlignment(TEXT_ALIGN_RIGHT);
            display.setFont(ArialMT_Plain_16);
            display.drawString(82, 15, stringify(tempC) + "°C");

            display.setFont(ArialMT_Plain_24);

            display.drawString(82, 32, stringify(tempRegulator.getTargetTemp()) + "°C");
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

        case DRAW_MESSAGE:
            display.setFont(ArialMT_Plain_16);
            display.setTextAlignment(TEXT_ALIGN_LEFT);
            display.drawString(0, 0, drawnMessage);
            break;

        default:

            break;

    }

    display.display();
}

void drawMessage(String message){
    drawnMessage = message;
    page = DRAW_MESSAGE;
    redraw();
}