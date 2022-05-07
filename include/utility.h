#ifndef STOCKS_UTILITY_H
#define STOCKS_UTILITY_H

#include <ctime>

bool operator==(const std::tm& t1, const std::tm& t2)
{
    return (t1.tm_sec == t2.tm_sec) &&
           (t1.tm_min == t2.tm_min) &&
           (t1.tm_hour == t2.tm_hour) &&
           (t1.tm_mday == t2.tm_mday) &&
           (t1.tm_mon == t2.tm_mon) &&
           (t1.tm_year == t2.tm_year) &&
           (t1.tm_isdst == t2.tm_isdst) &&
           (t1.tm_wday == t2.tm_wday) &&
           (t1.tm_yday == t2.tm_yday);
}

#endif //STOCKS_UTILITY_H
