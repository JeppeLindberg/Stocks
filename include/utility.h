#ifndef STOCKS_UTILITY_H
#define STOCKS_UTILITY_H

#include <iostream>
#include <fstream>
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

bool operator<=(const std::tm& t1, const std::tm& t2)
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
    return true;
}

std::ostream& operator<<(std::ostream& os, const std::tm& time) {
    return os << time.tm_year << "-" << time.tm_mon << "-" << time.tm_mday <<
              "T" << time.tm_hour << ":" << time.tm_min << ":" << time.tm_sec;
}

template<typename T>
constexpr auto slice(T&& container, int x, int y)
{
    return std::span(begin(std::forward<T>(container))+x, begin(std::forward<T>(container))+y);
}

struct utility_t{
    constexpr static std::tm min_tm{0,0,0,0,0,0,0,0,0};
    constexpr static std::tm max_tm{0,0,0,0,0,999,0,0,0};

    static std::tm tm_to_key(std::tm time, int hpc){
        time.tm_sec = 0;
        time.tm_min = 0;
        time.tm_hour = (time.tm_hour / hpc) * hpc;
        std::mktime(&time);
        return time;
    }

    static std::tm tm_move_key(std::tm time, int hpc, int move){
        time.tm_sec = 0;
        time.tm_min = 0;
        time.tm_hour += hpc * move;
        std::mktime(&time);
        return time;
    }

    static void set_time(const std::string& time_str, std::tm& time){
        std::istringstream ss(time_str);
        ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S.000+0100");
    }
};

#endif //STOCKS_UTILITY_H
