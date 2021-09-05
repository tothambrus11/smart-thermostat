#include "RotaryEncoder.h"

#ifndef SMART_THERM_ENCODER_H
#define SMART_THERM_ENCODER_H

class Encoder {
    bool aState, aLastState;
    int pinA, pinB;
    bool wasOnceBefore = false;
    std::function<void(bool scrolledDown)> onEvent;
    uint16_t state = 0;

public:

    Encoder(int pinA, int pinB);

    void setOnEventListener(std::function<void(bool scrolledDown)> onEvent);

    void onLoop();

};

#endif //SMART_THERM_ENCODER_H
