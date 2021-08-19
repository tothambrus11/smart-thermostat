//
// Created by ambru on 2021. 08. 18..
//
#include "RotaryEncoder.h"

#ifndef SMART_THERM_ENCODER_H
#define SMART_THERM_ENCODER_H

class Encoder {
    bool aState, aLastState;
    int pinA, pinB;
    bool wasOnceBefore=false;
    std::function<void(bool scrolledDown)> onEvent;
public:

    Encoder(int pinA, int pinB);

    void setOnEventListener(std::function<void(bool scrolledDown)> onEvent);

    void onLoop();
};

#endif //SMART_THERM_ENCODER_H
