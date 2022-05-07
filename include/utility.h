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

bool operator<(const std::tm& t1, const std::tm& t2)
{
    if (t1.tm_year != t2.tm_year)
        return (t1.tm_year < t2.tm_year);
    if (t1.tm_mon != t2.tm_mon)
        return (t1.tm_mon < t2.tm_mon);
    if (t1.tm_mday != t2.tm_mday)
        return (t1.tm_mday < t2.tm_mday);
    if (t1.tm_hour != t2.tm_hour)
        return (t1.tm_hour < t2.tm_hour);
    if (t1.tm_min != t2.tm_min)
        return (t1.tm_min < t2.tm_min);
    if (t1.tm_sec != t2.tm_sec)
        return (t1.tm_sec < t2.tm_sec);
    if (t1.tm_isdst != t2.tm_isdst)
        return (t1.tm_isdst < t2.tm_isdst);
    if (t1.tm_wday != t2.tm_wday)
        return (t1.tm_wday < t2.tm_wday);
    if (t1.tm_yday != t2.tm_yday)
        return (t1.tm_yday < t2.tm_yday);
    return false;
}

std::ostream& operator<<(std::ostream& os, const std::tm& time) {
    return os << time.tm_year << "-" << time.tm_mon << "-" << time.tm_mday <<
              "T" << time.tm_hour << ":" << time.tm_min << ":" << time.tm_sec;
}

struct utility_t{
    static std::tm modulo_tm(std::tm time, int mod){
        time.tm_sec = 0;
        time.tm_min = 0;
        time.tm_hour = (time.tm_hour / mod) * mod;
        return time;
    }
};

#endif //STOCKS_UTILITY_H
