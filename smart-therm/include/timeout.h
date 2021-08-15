#include "Arduino.h"

class Timeout {
   private:
    std::function<void()> callback = nullptr;

    bool ended = false;

   public:
    long endAt;

    Timeout(long timeout, std::function<void()> callback);

    bool check(long millis);

    static void checkAll();
};
