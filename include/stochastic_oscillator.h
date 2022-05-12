#ifndef STOCKS_STOCHASTIC_OSCILLATOR_H
#define STOCKS_STOCHASTIC_OSCILLATOR_H

#include "representation.h"
#include "utility.h"

struct point_t{
    double so; // K: The fast-moving stochastic oscillator
    double moving_avg; // D: The slow moving average over the last y values of K

    double closing_price; // C: The closing price of the previous period
    double lowest_overall; // L: The lowest overall price of the last x intervals
    double highest_overall; // H: The highest overall price of the last x intervals

    void calculate_so(){
        if(closing_price && lowest_overall && highest_overall)
            so = (closing_price - lowest_overall) / (highest_overall - lowest_overall);
        else
            so = -1;
    }
};

// Requirement 4
class stochastic_oscillator_t{
    representation_candle_t rc;

public:
    std::map<std::tm, point_t> points;
    int x = 14;
    int y = 3;

    bool get_period(const std::tm& start, const std::tm& end){
        std::map<std::tm, std::shared_ptr<interval_t>> intervals = rc.get_period(start, end);

        int intervals_size = intervals.size();
        int i = 0;
        bool first_iteration = true;
        std::tm first_key{};
        bool new_points = false;
        for(std::pair<std::tm, std::shared_ptr<interval_t>> pair : intervals){
            if(first_iteration){
                first_key = pair.first;
                first_iteration = false;
            }

            if (i > (intervals_size - x))
                break;

            std::tm key = utility_t::tm_move_key(pair.first, x);
            if(points.contains(key)) {
                i++;
                continue;
            }

            point_t point{};
            bool first_slice_loop = true;
            // Finds the span of x intervals for calculating lowest_overall and highest_overall
            auto slc = slice(rc.intervals, utility_t::tm_move_key(key, -x), key);
            for(std::pair<std::tm, std::shared_ptr<interval_t>> pair_prev : slc){
                if(first_slice_loop){
                    point.lowest_overall = pair_prev.second->min_price;
                    point.highest_overall = pair_prev.second->max_price;
                    first_slice_loop = false;
                }
                else {
                    if (pair_prev.second->min_price < point.lowest_overall)
                        point.lowest_overall = pair_prev.second->min_price;
                    if (point.highest_overall < pair_prev.second->max_price)
                        point.highest_overall = pair_prev.second->max_price;
                }
            }

            std::tm prev_key = utility_t::tm_move_key(key, -1);
            while (prev_key != first_key) {
                if (intervals.contains(prev_key)) {
                    point.closing_price = intervals.at(prev_key)->last_price;
                    break;
                }
                else
                    prev_key = utility_t::tm_move_key(prev_key, -1);
            }

            point.calculate_so();

            point.moving_avg = 0;
            if(i >= y) {
                prev_key = utility_t::tm_move_key(key, -1);
                int prev_closing_prices_added = 0;
                while (prev_closing_prices_added < y) {
                    if (points.contains(prev_key)) {
                        point.moving_avg += points[prev_key].so;
                        prev_closing_prices_added++;
                    }
                    prev_key = utility_t::tm_move_key(prev_key, -1);
                }
                point.moving_avg = point.moving_avg / y;
            }
            else
                point.moving_avg = -1;

            points[key] = point;
            new_points = true;

            i++;
        }

        return new_points;
    }

    double get_opening_price_after(std::tm time){
        return rc.get_opening_price_after(time);
    }

    stochastic_oscillator_t(const std::string& file_path) : rc{file_path}, points()
    {}
};

#endif //STOCKS_STOCHASTIC_OSCILLATOR_H
