#ifndef STOCKS_TRADING_H
#define STOCKS_TRADING_H

#include "stochastic_oscillator.h"

class trading_t{
    std::tm begin_time;
    std::map<std::tm, point_t>& points;

public:
    stochastic_oscillator_t so;
    std::tm time;

    double money = 10000;

    void simulate_trades(int duration_days) {
        begin_time = utility_t::tm_move_key(time, -20);
        int i = 0;
        while (i<duration_days){
            time = utility_t::tm_move_key(time, 1);

            points = so.get_period(begin_time, time);

            i++;
        }
    }

    trading_t(const std::string &file_path, std::tm _time): so(file_path), time(_time), points(points)
    {
        points = std::map<std::tm, point_t>{};
    }
};

#endif //STOCKS_TRADING_H
