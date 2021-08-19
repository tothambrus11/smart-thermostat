
#include "encoder.h"
#include "Arduino.h"

Encoder::Encoder(int pinA_, int pinB_) {
    pinA = pinA_;
    pinB = pinB_;
    aState = false;
    aLastState = false;

}

void Encoder::onLoop() {
    aState = digitalRead(pinA);

    if (aState != aLastState) {
        if(wasOnceBefore){
            bool scrolledDown = digitalRead(pinB) != aState;
            onEvent(scrolledDown);
        }
        wasOnceBefore = !wasOnceBefore;
    }
    aLastState = aState;
}

void Encoder::setOnEventListener(std::function<void(bool a)> onEvent_) {
    onEvent = onEvent_;
}
