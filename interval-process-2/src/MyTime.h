//
// Created by ambru on 2021. 09. 05..
//
#include "Arduino.h"
#ifndef INTERVAL_PROCESS_2_MYTIME_H
#define INTERVAL_PROCESS_2_MYTIME_H
struct TempInterval;

class MyTime {
public:
    int secondsFromMidnight;

    byte hours, minutes, seconds;

    MyTime(byte hours, byte minutes, byte seconds);

    MyTime();

    MyTime(tm *time);

    void init(byte hours_ = 0, byte minutes_ = 0, byte seconds_ = 0);

    bool operator<(const MyTime &rhs) const;

    bool operator>(const MyTime &rhs) const;

    bool operator<=(const MyTime &rhs) const;

    bool operator>=(const MyTime &rhs) const;

    String toString();
};

#endif //INTERVAL_PROCESS_2_MYTIME_H
