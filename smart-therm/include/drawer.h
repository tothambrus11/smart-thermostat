#include "Arduino.h"

#ifndef SMART_THERM_DRAWER_H
#define SMART_THERM_DRAWER_H
extern bool shouldRedraw;

void redraw();

void drawMessage(String message);

#endif //SMART_THERM_DRAWER_H
