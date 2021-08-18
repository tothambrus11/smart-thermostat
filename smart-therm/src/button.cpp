#include <Arduino.h>
#include "button.h"
#include "vector"

std::vector<Button*> buttons;

Button::Button(int _pinNumber) {
    pinNumber = _pinNumber;
    buttons.push_back(this);
}

void Button::checkAll() {
    for (unsigned int i = 0; i < buttons.size(); i++) {
        buttons[i]->check();
    }
}

void Button::setOnPressListener(std::function<void()> onPress) {
    this->onPress = onPress;
}

void Button::removeOnPressListener() {
    this->onPress = nullptr;
}

void Button::check() {
    this->prevIsPressed = isPressed;
    isPressed = !digitalRead(pinNumber);

    if (isPressed && !prevIsPressed && millis() - lastPressedAt > 250) {
        Serial.println(String(pinNumber) + "HWWWWWWW " + String(isPressed) + " " + String(prevIsPressed));

        if (onPress != nullptr) {
            Serial.println("ooooo");
            lastPressedAt = millis();
            onPress();
        }
    }
}