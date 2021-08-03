#include <Arduino.h>

class Timeout {
   private:
    std::function<void()> callback = nullptr;

    bool ended=false;
   public:
    long endAt;

    Timeout(long timeout, std::function<void()> callback);

    bool check(long millis) {
        if (!ended && endAt <= millis) {
            callback();
            ended=true;
            return true;
        }
        return false;
    }

    static void checkAll();
};

std::vector<Timeout *> _timeouts;

Timeout::Timeout(long timeout, std::function<void()> callback) {
    this->endAt = timeout + millis();
    this->callback = callback;
    _timeouts.push_back(this);
}

bool _hasChanged;

std::vector<Timeout *> _temp;

void Timeout::checkAll() {
    long _millis = millis();
    for (size_t i = 0; i < _timeouts.size(); i++) {
        _timeouts[i]->check(_millis);
    }
}
