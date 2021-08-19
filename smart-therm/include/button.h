#include <Arduino.h>

class Button {
   private:
    bool prevIsPressed = false;
    bool isPressed = false;
    int pinNumber;
    unsigned long lastPressedAt = 0;

    std::function<void()> onPress = nullptr;

   public:

    static std::vector<Button*> buttons; //ez alapbol nem egy ures vektor, hanem semmi

    void setOnPressListener(std::function<void()>);

    void removeOnPressListener();

    void check();

    void init();

    static void checkAll();

    explicit Button(int pinNumber);
};
