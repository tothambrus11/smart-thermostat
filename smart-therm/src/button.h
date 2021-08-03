#include <Arduino.h>

class Button
{
private:
    bool prevIsPressed = 0;
    int pinNumber;
    bool isPressed = 0;
    long lastPressedAt = 0;

    std::function<void()> onPress = nullptr;

public:
    void setOnPressListener(std::function<void()> onPress)
    {
        this->onPress = onPress;
    }

    void removeOnPressListener()
    {
        this->onPress = nullptr;
    }

    void check()
    {
        this->prevIsPressed = isPressed;
        isPressed = !digitalRead(pinNumber);

        if (isPressed && !prevIsPressed && millis() - lastPressedAt > 250)
        {
            Serial.println(String(pinNumber) + "HWWWWWWW " + String(isPressed) + " " + String(prevIsPressed));

            if (onPress != nullptr)
            {
                Serial.println("ooooo");
                lastPressedAt = millis();
                onPress();
            }
        }
    }

    static void checkAll();

    Button(int pinNumber);
};

std::vector<Button*> buttons;

Button::Button(int _pinNumber)
{
    pinNumber = _pinNumber;
    buttons.push_back(this);
}

void Button::checkAll()
{
    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->check();
    }
}