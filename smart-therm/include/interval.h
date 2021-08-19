#include <Arduino.h>

class Interval {
   private:
    std::function<void()> callback = nullptr;
    bool isStopped = false;

   public:
    static std::vector<Interval *> intervals;
    unsigned long interval;
    unsigned long lastOccurredAt = 0;

    void init(long interval, std::function<void()> callback);

    void stop();

    void restart();

    void check(unsigned long millis);

    static void checkAll();
};

