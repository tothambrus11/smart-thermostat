#include <interval_type.h>
#include <repetition_frequency.h>
#include "temp_interval_functions.h"
#include <chrono>
#include "Arduino.h"
#include "MyTime.h"
#include "TempInterval.h"
#include "storage.h"
#include "globals.h"

bool realNightMode = false; // based on time interval
bool actualNightMode; // affected by forced mode

std::vector<TempInterval> activeIntervals;
int activeIntervalOrder = -1;

bool isActiveTimeInterval(MyTime start, MyTime end, MyTime time) {
    if (start < end) {
        return start <= time && time < end;
    } else if (start > end) {
        return time > start or time < end;
    }
    return true;
}

bool isActiveDay(byte activeDaysOfWeek, byte currentDay) {
    return (activeDaysOfWeek >> (7 - currentDay)) & 1;
}

int dayOfWeek(const tm &t) {
    return t.tm_wday == 0 ? 7 : t.tm_wday;
}

bool inThePast(const tm &date, const tm &now) {
    if (date.tm_year != now.tm_year) return date.tm_year < now.tm_year;
    if (date.tm_mon != now.tm_mon) return date.tm_mon < now.tm_mon;
    if (date.tm_mday != now.tm_mday) return date.tm_mday < now.tm_mday;
    if (date.tm_hour != now.tm_hour) return date.tm_hour < now.tm_hour;
    if (date.tm_min != now.tm_min) return date.tm_min < now.tm_min;
    return date.tm_sec < now.tm_sec;
}

bool inTheFuture(const tm &date, const tm &now) {
    if (date.tm_year != now.tm_year) return date.tm_year > now.tm_year;
    if (date.tm_mon != now.tm_mon) return date.tm_mon > now.tm_mon;
    if (date.tm_mday != now.tm_mday) return date.tm_mday > now.tm_mday;
    if (date.tm_hour != now.tm_hour) return date.tm_hour > now.tm_hour;
    if (date.tm_min != now.tm_min) return date.tm_min > now.tm_min;
    return date.tm_sec > now.tm_sec;
}

bool inTheFutureOrNow(const tm &date, const tm &now) {
    return !inThePast(date, now);
}

bool inThePastOrNow(const tm &date, const tm &now) {
    return !inTheFuture(date, now);
}

bool isActiveDateInterval(const TempInterval &interval, const tm &currentDate) {
    tm startDate = {
            0,
            interval.startMinute,
            interval.startHour,
            interval.startDay,
            interval.startMonth - 1,
            interval.startYear - 1900
    };

    tm endDate = {
            0,
            interval.endMinute,
            interval.endHour,
            interval.endDay,
            interval.endMonth - 1,
            interval.endYear - 1900
    };

    return inThePastOrNow(startDate, currentDate) &&
           inTheFuture(endDate, currentDate);
}

void printIntervals(std::vector<TempInterval> &intervals) {
    for (const auto &item : intervals) {
        Serial.print(String(item.temperature) + "°C ");
        if (item.repetitionFrequency == RepetitionFrequency::NEVER) {
            Serial.print(String(item.startYear) + "." + String(item.startMonth) + "." + String(item.startDay) + " " +
                         item.startTime.toString() + " - " +
                         String(item.endYear) + "." + String(item.endMonth) + "." + String(item.endDay) + " " +
                         item.endTime.toString());
        } else {
            Serial.print(item.startTime.toString() + " - " + item.endTime.toString());
        }
        Serial.println();
    }
}

void checkAndActivateIntervals() {
    time_t timeTime;
    time(&timeTime);
    auto t = *localtime(&timeTime);

    MyTime nowMyTime(t);

    getCurrentlyActiveIntervals(t, nowMyTime);

    if (activeIntervals.empty()) {
        tempRegulator.setTargetTemp(storedData.normalTemp);
        activeIntervalOrder = -1;
    } else {
        tempRegulator.setTargetTemp(activeIntervals[0].temperature);
        activeIntervalOrder = activeIntervals[0].order;
    }

}

void setInitialIntervals() {
    TempInterval intervals_arr[] = {
            { // no
                    IntervalType::NIGHT,
                    17,
                    true,
                    RepetitionFrequency::DAILY,
                    0,
                    0,
                    0,
                    0,
                    22,
                    0,
                    0,
                    0,
                    0,
                    6,
                    0,
                    0,
            },
            {// yes
                    IntervalType::CUSTOM,
                    30,
                    true,
                    RepetitionFrequency::DAILY,
                    0,
                    0,
                    0,
                    0,
                    12,
                    30,
                    0,
                    0,
                    0,
                    17,
                    51,
                    1,
            },
            { // yes
                    IntervalType::CUSTOM,
                    32.6,
                    true,
                    RepetitionFrequency::DAILY,
                    0,
                    0,
                    0,
                    0,
                    16,
                    20,
                    0,
                    0,
                    0,
                    10,
                    0,
                    2
            },
            { // no
                    IntervalType::CUSTOM,
                    24.9,
                    true,
                    RepetitionFrequency::DAILY,
                    0,
                    0,
                    0,
                    0,
                    17,
                    20,
                    0,
                    0,
                    0,
                    3,
                    30,
                    3
            },
            { // yes
                    IntervalType::CUSTOM,
                    26.5,
                    true,
                    RepetitionFrequency::DAILY,
                    0,
                    0,
                    0,
                    0,
                    19,
                    10,
                    0,
                    0,
                    0,
                    17,
                    30,
                    4
            },
            { // yes
                    IntervalType::CUSTOM,
                    12.5,
                    true,
                    RepetitionFrequency::WEEKLY,
                    7,
                    0,
                    0,
                    0,
                    10,
                    10,
                    0,
                    9,
                    6,
                    19,
                    30,
                    5
            },
            { // no
                    IntervalType::CUSTOM,
                    10,
                    true,
                    RepetitionFrequency::WEEKLY,
                    6,
                    0,
                    0,
                    0,
                    10,
                    10,
                    0,
                    0,
                    0,
                    19,
                    30,
                    6
            },
            { // yes
                    IntervalType::CUSTOM,
                    12.6,
                    true,
                    RepetitionFrequency::NEVER,
                    0,
                    2021,
                    9,
                    5,
                    20,
                    30,
                    2021,
                    9,
                    13,
                    20,
                    47,
                    7
            },
            { // no
                    IntervalType::CUSTOM,
                    11.5,
                    true,
                    RepetitionFrequency::NEVER,
                    0,
                    2020,
                    9,
                    5,
                    18,
                    50,
                    2020,
                    9,
                    6,
                    19,
                    30,
                    8
            }
    };
    tempIntervals.clear();

    for (TempInterval interval_ : intervals_arr) {
        tempIntervals.push_back(interval_);
    }
}

bool isActiveInterval(const TempInterval &interval, const tm &t, const MyTime &now, const byte &currentDay) {
    if (!interval.enabled) return false;
    switch (interval.type) {
        case IntervalType::CUSTOM:
            switch (interval.repetitionFrequency) {
                case RepetitionFrequency::DAILY:
                    return isActiveTimeInterval(interval.startTime, interval.endTime, now);
                case RepetitionFrequency::WEEKLY:
                    return (isActiveTimeInterval(interval.startTime, interval.endTime, now) &&
                            isActiveDay(interval.daysOfWeek, currentDay));
                case RepetitionFrequency::NEVER:
                    return isActiveDateInterval(interval, t);
            }

            break;
        case IntervalType::NIGHT:
            bool realNightMode2;

            if (interval.startTime < interval.endTime) {
                realNightMode2 = interval.startTime <= now && now < interval.endTime;
            } else {
                realNightMode2 = now > interval.startTime or now < interval.endTime;
            }

            if (realNightMode2 != realNightMode) { // interval end or beginning
                if (storedData.forcedDayMode || storedData.forcedNightMode) shouldSave = true;
                storedData.forcedNightMode = false;
                storedData.forcedDayMode = false;
            }

            // affected by forced night and day mode
            actualNightMode = realNightMode2 || storedData.forcedNightMode;
            actualNightMode = actualNightMode && !storedData.forcedDayMode;

            realNightMode = realNightMode2;

            return actualNightMode;
    }
    return false;
}

void getCurrentlyActiveIntervals(const tm &t, const MyTime &now) {
    byte currentDay = dayOfWeek(t);

    activeIntervals.clear();
    activeIntervals.reserve(20);

    for (auto &interval : tempIntervals) {
        if (isActiveInterval(interval, t, now, currentDay)) {
            activeIntervals.push_back(interval);
        }
        if (interval.type == IntervalType::NIGHT && !interval.enabled) {
            actualNightMode = false;
        }
    }
}

void changeNightMode() {
    if (actualNightMode) { // should be day mode
        if (realNightMode) { // éjjeli mód éjszaka
            storedData.forcedDayMode = true;
        } else { // éjjeli mód nappal
            storedData.forcedNightMode = false;
        }
    } else { // should be night mode
        if (realNightMode) { // nappali mód éjszaka
            storedData.forcedDayMode = false;
        } else { // nappali mód nappal
            storedData.forcedNightMode = true;
        }
    }
    shouldSave = true;
}

