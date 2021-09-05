#include <Arduino.h>
#include "button.h"

std::vector<Button *> Button::buttons = std::vector<Button *>();

Button::Button(int _pinNumber) {
    pinNumber = _pinNumber;
}

void Button::init(){
    Button::buttons.push_back(this);
}

void Button::checkAll() {
    for (auto &button : Button::buttons) {
        button->check();
    }
}

void Button::setOnPressListener(std::function<void()> onPress_) {
    this->onPress = std::move(onPress_);
}

void Button::removeOnPressListener() {
    this->onPress = nullptr;
}

void Button::check() {
    prevIsPressed = isPressed;
    isPressed = !digitalRead(pinNumber);

    if (isPressed && !prevIsPressed && millis() - lastPressedAt > 250) {
        if (onPress != nullptr) {
            lastPressedAt = millis();
            onPress();
        }
    }
}