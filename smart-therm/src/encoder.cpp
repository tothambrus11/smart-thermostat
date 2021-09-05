#include "encoder.h"
#include "Arduino.h"

Encoder::Encoder(int pinA_, int pinB_) {
    pinA = pinA_;
    pinB = pinB_;
    aState = false;
    aLastState = false;

}

void Encoder::onLoop() {
    state = (state << 1) | digitalRead(pinA) | 0xe000;

    if (state == 0xf000) {
        state = 0x0000;
        if (digitalRead(pinB))
            onEvent(true);
        else
            onEvent(false);
    }
}

void Encoder::setOnEventListener(std::function<void(bool a)> onEvent_) {
    onEvent = std::move(onEvent_);
}
