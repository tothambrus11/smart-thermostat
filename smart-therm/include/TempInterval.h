#pragma once

#include "MyTime.h"

struct TempInterval{
    byte type{}; // IntervalType
    float temperature{};
    bool enabled{};
    byte repetitionFrequency{}; // RepetitionFrequency
    byte daysOfWeek{};
    int startYear{};
    byte startMonth{};
    byte startDay{};
    byte startHour{};
    byte startMinute{};
    int endYear{};
    byte endMonth{};
    byte endDay{};
    byte endHour{};
    byte endMinute{};
    int order{};
    MyTime startTime{};
    MyTime endTime{};
};

