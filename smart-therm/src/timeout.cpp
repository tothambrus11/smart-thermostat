#include "timeout.h"
#include <Arduino.h>

std::vector<Timeout *> _timeouts;
bool _hasChanged;
std::vector<Timeout *> _temp;


Timeout::Timeout(long timeout, std::function<void()> callback) {
    this->endAt = timeout + millis();
    this->callback = callback;
    _timeouts.push_back(this);
}

void Timeout::checkAll() {
    long _millis = millis();
    for (size_t i = 0; i < _timeouts.size(); i++) {
        _timeouts[i]->check(_millis);
    }
}

bool Timeout::check(long millis) {
    if (!ended && endAt <= millis) {
        callback();
        ended = true;
        return true;
    }
    return false;
}
