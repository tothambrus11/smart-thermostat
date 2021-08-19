#include "interval.h"
#include "vector"

std::vector<Interval *> Interval::intervals = std::vector<Interval *>();

void Interval::init(long interval_, std::function<void()> callback_) {
    callback = std::move(callback_);
    interval = interval_;
    intervals.push_back(this);
}

void Interval::checkAll() {
    unsigned long millis_ = millis();
    for (auto &interval : Interval::intervals) {
        interval->check(millis_);
    }
}

void Interval::stop() {
    isStopped = true;
}

void Interval::restart() {
    isStopped = false;
}

void Interval::check(unsigned long millis) {
    if (!isStopped && lastOccurredAt + interval <= millis) {
        lastOccurredAt = millis;
        callback();
    }
}