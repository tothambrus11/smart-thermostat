#include <Arduino.h>

class Interval
{
private:
    std::function<void()> callback = nullptr;
    bool isStopped = false;

public:
    long interval;
    long lastOccurredAt = 0;

    void init(long interval, std::function<void()> callback);

    void stop()
    {
        isStopped = true;
    }

    void restart()
    {
        isStopped = false;
    }

    void check(long millis)
    {
        if (!isStopped && lastOccurredAt + interval <= millis)
        {
            lastOccurredAt = millis;
            callback();
        }
    }

    static void checkAll();
};

std::vector<Interval *> _intervals;

void Interval::init(long interval, std::function<void()> callback) {
    this->callback = callback;
    this->interval = interval;
    _intervals.push_back(this);
}

void Interval::checkAll(){
    long millis_ = millis();
    for (size_t i = 0; i < _intervals.size(); i++)
    {
        _intervals[i]->check(millis_);
    }
}