#ifndef STOCKS_UTILITY_H
#define STOCKS_UTILITY_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <map>

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
    return os << time.tm_year+1900 << "-" << time.tm_mon+1 << "-" << time.tm_mday <<
              "T" << time.tm_hour << ":" << time.tm_min << ":" << time.tm_sec;
}

struct utility_t{
    constexpr static std::tm min_tm{0,0,0,0,0,0,0,0,0};
    constexpr static std::tm max_tm{0,0,0,0,0,9999,0,0,0};
    constexpr static int hpc = 24; // Hours per candle

    static std::tm tm_to_key(std::tm time){
        time.tm_sec = 0;
        time.tm_min = 0;
        time.tm_hour = (time.tm_hour / hpc) * hpc;
        std::mktime(&time);
        return time;
    }

    static std::tm tm_move_key(std::tm time, int move){
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

template<typename T>
constexpr auto slice(T&& container, int from, int to)
{
    return std::span(begin(std::forward<T>(container))+from, begin(std::forward<T>(container))+to);
}

template<typename T>
constexpr std::map<std::tm, T> slice(std::map<std::tm, T>& map, std::tm from, std::tm to)
{
    // Possible point of optimization: Prevent the slice from being copies, and instead make them references

    std::tm key = from;
    std::map<std::tm, T> ret_map;

    while(key < to){
        if (map.contains(key))
            ret_map[key] = map[key];
        key = utility_t::tm_move_key(key, 1);
    }

    return ret_map;
}

template<typename T>
constexpr bool contains_slice(std::map<std::tm, T>& map, const std::tm& from, const std::tm& to)
{
    bool contains_lower = false;
    bool contains_higher = false;

    for(const std::pair<std::tm, T>& pair : map){
        if(!contains_lower && pair.first <= from)
            contains_lower = true;
        if(!contains_higher && to <= pair.first)
            contains_higher = true;
        if(contains_lower && contains_higher)
            return true;
    }
    return false;
}

#endif //STOCKS_UTILITY_H
