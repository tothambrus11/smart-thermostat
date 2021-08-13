# Memory usage

## Temp Intervals (20 byte / interval, minimum 2 intervals)
* 1 byte - type (normal / night / custom)
* 4 byte float - temp
* 1 byte char - enabled
* 4 byte 4xchar - interval
* 1 byte days of week
* 1 byte repetition frequency (daily, weekly, never)
* 8 byte date interval