#include "DateTimeLib.h"

const uint8_t daysArray[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint32_t DateTimeLib::unixtime(const char *datetime)
{
  return DateTimeLib::unixtime(String(datetime));
}

uint32_t DateTimeLib::unixtime(String datetime)
{
  uint16_t year = (uint16_t) datetime.substring(0, 4).toInt();
  uint8_t month = (uint8_t) datetime.substring(5, 7).toInt();
  uint8_t day = (uint8_t) datetime.substring(8, 10).toInt();
  uint8_t hour = (uint8_t) datetime.substring(11, 13).toInt();
  uint8_t minute = (uint8_t) datetime.substring(14, 16).toInt();
  uint8_t second = (uint8_t) datetime.substring(17, 19).toInt();

  return DateTimeLib::unixtime(year, month, day, hour, minute, second);
}

uint32_t DateTimeLib::unixtime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
  uint32_t u;

  u = time2long(date2days(year, month, day), hour, minute, second);
  u += 946681200;

  return u;
}

uint16_t DateTimeLib::date2days(uint16_t year, uint8_t month, uint8_t day)
{
  year = year - 2000;

  uint16_t days16 = day;

  for (uint8_t i = 1; i < month; ++i)
  {
    days16 += pgm_read_byte(daysArray + i - 1);
  }

  if ((month == 2) && isLeapYear(year))
  {
    ++days16;
  }

  return days16 + 365 * year + (year + 3) / 4 - 1;
}

long DateTimeLib::time2long(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  return ((days * 24L + hours) * 60 + minutes) * 60 + seconds;
}

uint16_t DateTimeLib::dayInYear(uint16_t year, uint8_t month, uint8_t day)
{
  uint16_t fromDate;
  uint16_t toDate;

  fromDate = date2days(year, 1, 1);
  toDate = date2days(year, month, day);

  return (toDate - fromDate);
}

bool DateTimeLib::isLeapYear(uint16_t year)
{
  return (year % 4 == 0);
}
