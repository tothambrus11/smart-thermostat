#include "Arduino.h"
#ifndef INTERVAL_PROCESS_2_MYTIME_H
#define INTERVAL_PROCESS_2_MYTIME_H

class MyTime {
public:
    MyTime();

    int secondsFromMidnight;

    byte hours, minutes, seconds;

    MyTime(byte hours, byte minutes, byte seconds = 0);

    explicit MyTime(const tm &time);

    void init(byte hours_ = 0, byte minutes_ = 0, byte seconds_ = 0);

    bool operator<(const MyTime &rhs) const;

    bool operator>(const MyTime &rhs) const;

    bool operator<=(const MyTime &rhs) const;

    bool operator>=(const MyTime &rhs) const;

    String toString() const;
};

#endif //INTERVAL_PROCESS_2_MYTIME_H
