//
// Created by ambru on 2021. 09. 05..
//

#include "MyTime.h"

bool MyTime::operator<(const MyTime &rhs) const {
    return secondsFromMidnight < rhs.secondsFromMidnight;
}

bool MyTime::operator>(const MyTime &rhs) const {
    return rhs < *this;
}

bool MyTime::operator<=(const MyTime &rhs) const {
    return !(rhs < *this);
}

bool MyTime::operator>=(const MyTime &rhs) const {
    return !(*this < rhs);
}

String MyTime::toString() {
    return String(hours) + ":" + String(minutes) + ":" + String(seconds);
}

void MyTime::init(byte hours_, byte minutes_, byte seconds_) {
    secondsFromMidnight = hours_ * 3600 + minutes_ * 60 + seconds_;
    this->hours = hours_;
    this->minutes = minutes_;
    this->seconds = seconds_;
}

MyTime::MyTime(tm *time) {
    hours = time->tm_hour;
    minutes = time->tm_min;
    seconds = time->tm_sec;
    secondsFromMidnight = hours * 3600 + minutes * 60 + seconds;
}

MyTime::MyTime() {
    MyTime(getLocalTime());
}

MyTime::MyTime(byte hours, byte minutes, byte seconds) {
    secondsFromMidnight = hours * 3600 + minutes * 60 + seconds;
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;
}
