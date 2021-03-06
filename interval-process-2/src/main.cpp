#include <chrono>
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "ESPDateTime.h"
#include "MyTime.h"
#include "TempInterval.h"


struct WifiCredentials {
    byte ssid[32];
    byte password[32];
    unsigned short ssidLength;
    unsigned short passwordLength;
};

struct StoredData {
    WifiCredentials wifiAP;
    WifiCredentials connectedWifi;
    unsigned int timezoneOffset; // minutes
    unsigned int dstOffset; // daylight saving time minutes

    unsigned int corruptionCheck;
};

class IntervalType {
public:
    static const int NIGHT = 0, CUSTOM = 1;
};

class RepetitionFrequency {
public:
    static const int DAILY = 0, WEEKLY = 1, NEVER = 2;
};

void getCurrentlyActiveIntervals(const std::vector<TempInterval *> &intervals, tm *t,
                                 std::vector<TempInterval *> activeIntervals, MyTime now);

bool isActiveTimeInterval(MyTime start, MyTime end, MyTime time) {
    if (start < end) {
        return start <= time && time < end;
    } else if (start > end) {
        return time > start or time < end;
    }
    return true;
}

// monday is 1
bool isActiveDay(byte activeDaysOfWeek, byte currentDay) {
    return (activeDaysOfWeek >> (7 - currentDay)) & 1;
}

int dayOfWeek(tm *t) {
    return t->tm_wday == 0 ? 7 : t->tm_wday;
}

inline bool inThePast(tm date, tm now) {
    if (date.tm_year != now.tm_year) return date.tm_year < now.tm_year;
    if (date.tm_mon != now.tm_mon) return date.tm_mon < now.tm_mon;
    if (date.tm_mday != now.tm_mday) return date.tm_mday < now.tm_mday;
    if (date.tm_hour != now.tm_hour) return date.tm_hour < now.tm_hour;
    if (date.tm_min != now.tm_min) return date.tm_min < now.tm_min;
    return date.tm_sec < now.tm_sec;
}

inline bool inTheFuture(tm date, tm now) {
    if (date.tm_year != now.tm_year) return date.tm_year > now.tm_year;
    if (date.tm_mon != now.tm_mon) return date.tm_mon > now.tm_mon;
    if (date.tm_mday != now.tm_mday) return date.tm_mday > now.tm_mday;
    if (date.tm_hour != now.tm_hour) return date.tm_hour > now.tm_hour;
    if (date.tm_min != now.tm_min) return date.tm_min > now.tm_min;
    return date.tm_sec > now.tm_sec;
}

inline bool inTheFutureOrNow(tm date, tm now) {
    return !inThePast(date, now);
}

inline bool inThePastOrNow(tm date, tm now) {
    return !inTheFuture(date, now);
}

bool isActiveDateInterval(TempInterval *interval, tm *currentDate) {
    tm startDate = {
            0,
            interval->startMinute,
            interval->startHour,
            interval->startDay,
            interval->startMonth - 1,
            interval->startYear - 1900
    };

    tm endDate = {
            0,
            interval->endMinute,
            interval->endHour,
            interval->endDay,
            interval->endMonth - 1,
            interval->endYear - 1900
    };

    return inThePastOrNow(startDate, *currentDate) &&
           inTheFuture(endDate, *currentDate);
}

void printIntervals(std::vector<TempInterval *> &intervals) {
    for (const auto &item : intervals) {
        Serial.print(String(item->temperature) + "??C ");
        if (item->repetitionFrequency == RepetitionFrequency::NEVER) {
            Serial.print(String(item->startYear) + "." + String(item->startMonth) + "." + String(item->startDay) + " " +
                         item->startTime.toString() + " - " +
                         String(item->endYear) + "." + String(item->endMonth) + "." + String(item->endDay) + " " +
                         item->endTime.toString());
        } else {
            Serial.print(item->startTime.toString() + " - " + item->endTime.toString());
        }
        Serial.println();
    }
}

std::vector<TempInterval *> getIntervals() {
    TempInterval intervals_arr[] = {
            { // no
                    IntervalType::CUSTOM,
                    24.5,
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
                    15,
                    32,
                    1
            },
            {// yes
                    IntervalType::CUSTOM,
                    24.5,
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
                    2
            },
            { // yes
                    IntervalType::CUSTOM,
                    24.5,
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
                    24.5,
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
                    2
            },
            { // yes
                    IntervalType::CUSTOM,
                    24.5,
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
                    2
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
                    2
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
                    2
            },
            { // yes
                    IntervalType::CUSTOM,
                    12.5,
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
                    5,
                    20,
                    47,
                    2
            },

            { // no
                    IntervalType::CUSTOM,
                    12.5,
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
                    2
            },
    };

    std::vector<TempInterval *> intervals;
    for (TempInterval &interval_ : intervals_arr) {
        intervals.push_back(&interval_);
    }
    return intervals;
}

bool forcedNightMode;
bool forcedDayMode;
bool actualNightMode;

void setup() {
    Serial.begin(9600);

    WiFi.begin("TelePort Link", "Sajtkukac2004");
    WiFi.waitForConnectResult(10000);
    if (WiFi.isConnected()) {
        Serial.println("Connected");
    } else {
        Serial.println("Not connected");
    }

    DateTime.setTimeZone("CET-1CEST,M3.5.0,M10.5.0/3");
    DateTime.begin();
    if (!DateTime.isTimeValid()) {
        Serial.println("Failed to get time from server.");
    }

    time_t timeTime;
    time(&timeTime);
    auto t = localtime(&timeTime);

    MyTime nowMyTime(t);

    auto intervals = getIntervals();

    for (auto &interval : intervals) {
        interval->startTime.init(interval->startHour, interval->startMinute);
        interval->endTime.init(interval->endHour, interval->endMinute);
    }

    std::vector<TempInterval *> activeIntervals;
    getCurrentlyActiveIntervals(intervals, t, activeIntervals, nowMyTime);

    Serial.println("Active intervals: ");
    printIntervals(activeIntervals);
}

bool realNightMode = false;

void getCurrentlyActiveIntervals(const std::vector<TempInterval *> &intervals, tm *t,
                                 std::vector<TempInterval *> activeIntervals, MyTime now) {
    byte currentDay = dayOfWeek(t);
    activeIntervals.clear();

    for (auto &interval : intervals) {
        if (!interval->enabled) continue;
        switch (interval->type) {
            case IntervalType::CUSTOM:
                switch (interval->repetitionFrequency) {
                    case RepetitionFrequency::DAILY:
                        if (isActiveTimeInterval(interval->startTime, interval->endTime, now)) {
                            activeIntervals.push_back(interval);
                        }
                        break;
                    case RepetitionFrequency::WEEKLY:
                        if (isActiveTimeInterval(interval->startTime, interval->endTime, now) &&
                            isActiveDay(interval->daysOfWeek, currentDay)) {
                            activeIntervals.push_back(interval);
                        }
                        break;
                    case RepetitionFrequency::NEVER:
                        if (isActiveDateInterval(interval, t)) {
                            activeIntervals.push_back(interval);
                        }
                        break;
                }

                break;
            case IntervalType::NIGHT:
                bool realNightMode2;

                if (interval->startTime < interval->endTime) {
                    realNightMode2 = interval->startTime <= now && now < interval->endTime;
                } else {
                    realNightMode2 = now > interval->startTime or now < interval->endTime;
                }

                if (realNightMode2 != realNightMode) { // interval end or beginning
                    forcedNightMode = false;
                    forcedDayMode = false;
                }

                // affected by forced night and day mode
                actualNightMode = realNightMode2 || forcedNightMode;
                actualNightMode = actualNightMode && !forcedDayMode;

                realNightMode = realNightMode2;

                if (actualNightMode) {
                    activeIntervals.push_back(interval);
                }
                break;
        }
    }
}

TempInterval *getCurrentInterval(const std::vector<TempInterval *> &activeIntervals) {
    if (activeIntervals.empty()) return nullptr;

    TempInterval *bestInterval = activeIntervals[0];
    for (auto interval : activeIntervals) {
        if(bestInterval->order < interval->order){
            bestInterval = interval;
        }
    }
    return bestInterval;
}

void changeNightMode() {
    if (actualNightMode) { // should be day mode
        if (realNightMode) { // ??jjeli m??d ??jszaka
            forcedDayMode = true;
        } else { // ??jjeli m??d nappal
            forcedNightMode = false;
        }
    } else { // should be night mode
        if (realNightMode) { // nappali m??d ??jszaka
            forcedDayMode = false;
        } else { // nappali m??d nappal
            forcedNightMode = true;
        }
    }
}

void loop() {

}