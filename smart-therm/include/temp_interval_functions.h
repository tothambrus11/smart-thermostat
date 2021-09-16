#ifndef SMART_THERM_TEMP_INTERVAL_FUNCTIONS_H
#define SMART_THERM_TEMP_INTERVAL_FUNCTIONS_H

#include "Arduino.h"
#include "MyTime.h"
#include "TempInterval.h"

extern bool forcedNightMode;
extern bool forcedDayMode;
extern bool realNightMode; // based on time interval
extern bool actualNightMode; // affected by forced mode


bool isActiveTimeInterval(MyTime start, MyTime end, MyTime time);

// monday is 1
bool isActiveDay(byte activeDaysOfWeek, byte currentDay);

bool isActiveDay(byte activeDaysOfWeek, byte currentDay);

int dayOfWeek(tm *t);

inline bool inThePast(tm date, tm now);

inline bool inTheFuture(tm date, tm now);

inline bool inTheFutureOrNow(tm date, tm now);

inline bool inThePastOrNow(tm date, tm now);

bool isActiveDateInterval(TempInterval *interval, tm *currentDate);

void printIntervals(std::vector<TempInterval *> &intervals);

void getInitialIntervals(std::vector<TempInterval*> &ivs);

void getCurrentlyActiveIntervals(const std::vector<TempInterval *> &intervals, tm *t,
                                 std::vector<TempInterval *> &activeIntervals, MyTime now);

TempInterval *getCurrentInterval(const std::vector<TempInterval *> &activeIntervals);

void changeNightMode();

void checkAndActivateIntervals();

void saveIntervalVector(std::vector<TempInterval *> * intervals);

#endif //SMART_THERM_TEMP_INTERVAL_FUNCTIONS_H
