//
// Created by ambru on 2021. 09. 05..
//

#ifndef SMART_THERM_MY_TIME_H
#define SMART_THERM_MY_TIME_H
class MyTime {
public:
    int secondsFromMidnight;

    char hour, minute, seconds;

    MyTime(char hour, char minute, char seconds) {
        secondsFromMidnight = hour * 3600 + minute * 60 + seconds;
        this->hour = hour;
        this->minute = minute;
        this->seconds = seconds;
    }

    bool operator<(const MyTime &rhs) const {
        return secondsFromMidnight < rhs.secondsFromMidnight;
    }

    bool operator>(const MyTime &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const MyTime &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const MyTime &rhs) const {
        return !(*this < rhs);
    }
};

bool isActiveTimeInterval(MyTime start, MyTime end, MyTime time) {
    if (start < end) {
        return start <= time && time < end;
    } else if (start > end) {
        return time > start or time < end;
    }
    return true;
}

// monday is 0
bool isActiveDay(char activeDaysOfWeek, char currentDay) {
    return (activeDaysOfWeek >> (6 - currentDay)) & 1;
}
#endif //SMART_THERM_MY_TIME_H
