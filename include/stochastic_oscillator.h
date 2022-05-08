#ifndef STOCKS_STOCHASTIC_OSCILLATOR_H
#define STOCKS_STOCHASTIC_OSCILLATOR_H

#include "representation.h"
#include "utility.h"

struct point_t{
    double k;
    double d;

    double c;
    double l;
    double h;
};

// Requirement 4
class stochastic_oscillator_t{
public:
    int x = 14;
    int y = 3;
    representation_candle_t rc;
    std::vector<point_t> points;

    std::vector<point_t> get_period(const std::tm& start, const std::tm& end){
        std::vector<interval_t> intervals = rc.get_period(start, end);
        points = std::vector<point_t>(intervals.size() - x);

        int i = 0;
        for(point_t& point : points){
            bool first_slice_loop = true;
            for(const interval_t& interval : slice(intervals, i, i+x)){
                if(first_slice_loop){
                    point.l = interval.min_price;
                    point.h = interval.max_price;
                    first_slice_loop = false;
                }
                else {
                    if (interval.min_price < point.l)
                        point.l = interval.min_price;
                    if (point.h < interval.max_price)
                        point.h = interval.max_price;
                }
            }
            point.c = (intervals.begin()+i+x-1)->last_price;

            point.k = (point.c - point.l)/(point.h - point.l);

            if(i >= y) {
                for(const point_t& point_2 : slice(points, i-y, i-1)){
                    point.d += point_2.k;
                }
                point.d = point.d/y;
            }

            i++;
        }

        return points;
    }

    stochastic_oscillator_t(const std::string& file_path) : rc{file_path}
    {}
};

#endif //STOCKS_STOCHASTIC_OSCILLATOR_H
