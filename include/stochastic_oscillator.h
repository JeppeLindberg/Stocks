#ifndef STOCKS_STOCHASTIC_OSCILLATOR_H
#define STOCKS_STOCHASTIC_OSCILLATOR_H

#include "representation.h"
#include "utility.h"

struct point_t{
    double so; // K: The fast stochastic oscillator
    double moving_avg; // D: The slow moving average over the last y values of K

    double closing_price; // C
    double lowest_overall; // L: The lowest overall price of the last x intervals
    double highest_overall; // H: The highest overall price of the last x intervals
};

// Requirement 4
class stochastic_oscillator_t{
    representation_candle_t rc;

public:
    int x = 14;
    int y = 3;
    std::map<std::tm, point_t> points;

    std::map<std::tm, point_t> get_period(const std::tm& start, const std::tm& end){
        std::map<std::tm, interval_t> intervals = rc.get_period(start, end);

        int intervals_size = intervals.size();
        int i = 0;
        double last_c = 0;
        for(std::pair<std::tm, interval_t> pair : intervals){
            if (i > (intervals_size - x))
                break;

            std::tm key = utility_t::tm_move_key(pair.first, rc.hpc, x);
            if (points.contains(key))
                continue;

            point_t point{};
            bool first_slice_loop = true;
            // Finds the span of x intervals for calculating lowest_overall and highest_overall
            auto slc = slice(intervals, utility_t::tm_move_key(key, rc.hpc, -x),
                               key,
                               rc.hpc);
            for(std::pair<std::tm, interval_t> pair_prev : slc){
                if(first_slice_loop){
                    point.lowest_overall = pair_prev.second.min_price;
                    point.highest_overall = pair_prev.second.max_price;
                    first_slice_loop = false;
                }
                else {
                    if (pair_prev.second.min_price < point.lowest_overall)
                        point.lowest_overall = pair_prev.second.min_price;
                    if (point.highest_overall < pair_prev.second.max_price)
                        point.highest_overall = pair_prev.second.max_price;
                }
            }
            std::tm prev_key = utility_t::tm_move_key(key, rc.hpc, -1);

            if (intervals.contains(prev_key)){
                point.closing_price = intervals[prev_key].last_price;
                last_c = point.closing_price;
            }
            else
                point.closing_price = last_c;

            point.so = (point.closing_price - point.lowest_overall) / (point.highest_overall - point.lowest_overall);

            points[key] = point;

            auto slc_2 = slice(points,
                               utility_t::tm_move_key(key, rc.hpc, -y),
                               utility_t::tm_move_key(key, rc.hpc, -1),
                               rc.hpc);
            if(i >= y) {
                for(const std::pair<std::tm, point_t> prev_point : slc_2){
                    point.moving_avg += prev_point.second.so;
                }
                point.moving_avg = point.moving_avg / y;
            }

            i++;
        }

        return points;
    }

    stochastic_oscillator_t(const std::string& file_path) : rc{file_path}, points()
    {}
};

#endif //STOCKS_STOCHASTIC_OSCILLATOR_H
