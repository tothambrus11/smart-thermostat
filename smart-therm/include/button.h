#include <Arduino.h>

class Button {
   private:
    bool prevIsPressed = 0;
    int pinNumber;
    bool isPressed = 0;
    unsigned long lastPressedAt = 0;

    std::function<void()> onPress = nullptr;

   public:
    void setOnPressListener(std::function<void()> onPress);

    void removeOnPressListener();

    void check();

    static void checkAll();

    Button(int pinNumber);
};
