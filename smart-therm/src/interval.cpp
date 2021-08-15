#include "interval.h"

std::vector<Interval *> _intervals;

void Interval::init(long interval, std::function<void()> callback) {
    this->callback = callback;
    this->interval = interval;
    _intervals.push_back(this);
}

void Interval::checkAll() {
    long millis_ = millis();
    for (size_t i = 0; i < _intervals.size(); i++) {
        _intervals[i]->check(millis_);
    }
}

void Interval::stop() {
    isStopped = true;
}

void Interval::restart() {
    isStopped = false;
}

void Interval::check(long millis) {
    if (!isStopped && lastOccurredAt + interval <= millis) {
        lastOccurredAt = millis;
        callback();
    }
}