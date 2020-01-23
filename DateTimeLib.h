#ifndef DateTimeLib_h
#define DateTimeLib_h

#include "Arduino.h"

class DateTimeLib
{
public:
    static uint32_t unixtime(const char* datetime); //format yyyy-mm-dd HH:mm:ss
    static uint32_t unixtime(String datetime); //format yyyy-mm-dd HH:mm:ss
    static uint32_t unixtime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    static uint16_t date2days(uint16_t year, uint8_t month, uint8_t day);
    static long time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
    static uint16_t dayInYear(uint16_t year, uint8_t month, uint8_t day);
    static bool isLeapYear(uint16_t year);
};
#endif
