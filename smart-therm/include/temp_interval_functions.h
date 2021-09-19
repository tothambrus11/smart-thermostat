#pragma once

#include "Arduino.h"
#include "MyTime.h"
#include "TempInterval.h"

extern bool realNightMode; // based on time interval
extern bool actualNightMode; // affected by forced mode
extern int activeIntervalOrder;

bool isActiveTimeInterval(MyTime start, MyTime end, MyTime time);

// monday is 1
bool isActiveDay(byte activeDaysOfWeek, byte currentDay);

bool isActiveDay(byte activeDaysOfWeek, byte currentDay);

int dayOfWeek(const tm &t);

bool inThePast(const tm &date, const tm &now);

bool inTheFuture(const tm &date,const tm &now);

bool inTheFutureOrNow(const tm &date, const tm &now);

bool inThePastOrNow(const tm &date, const tm &now);

bool isActiveDateInterval(const TempInterval &interval, const tm &currentDate);

void printIntervals(std::vector<TempInterval> &intervals);

void setInitialIntervals();

void getCurrentlyActiveIntervals(const tm &t, const MyTime &now);

TempInterval *getCurrentInterval();

void changeNightMode();

void checkAndActivateIntervals();
