#include <Arduino.h>

class Interval {
   private:
    std::function<void()> callback = nullptr;
    bool isStopped = false;

   public:
    long interval;
    long lastOccurredAt = 0;

    void init(long interval, std::function<void()> callback);

    void stop();

    void restart();

    void check(long millis);

    static void checkAll();
};

