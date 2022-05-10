#ifndef STOCKS_TRADING_H
#define STOCKS_TRADING_H

#include <functional>
#include "stochastic_oscillator.h"

enum Action{
    do_nothing = 0,
    buy = 1,
    sell = 2,
};

// Requirement 5, requirement 6
class trading_t{
    std::tm begin_time;

    void buy_stock(const std::tm& time){
        double price = so.get_opening_price_after(time);
        if(money > 0)
            std::cout << "Buy at " << price << " (" << time << ")" << std::endl;
        shares += money/price;
        money = 0;
    }

    void sell_stock(const std::tm& time){
        double price = so.get_opening_price_after(time);
        if(shares > 0)
            std::cout << "Sell at " << price << " (" << time << ")" << std::endl;
        money += shares*price;
        shares = 0;
    }

public:
    std::map<std::tm, point_t> points;
    stochastic_oscillator_t so;
    std::tm current_time;
    std::function<Action(point_t)> trade_strategy;

    double money = 10000;
    double shares = 0;

    void simulate_trades(int duration_days) {
        begin_time = utility_t::tm_move_key(current_time, -20);
        int i = 0;
        while (i<duration_days){
            current_time = utility_t::tm_move_key(current_time, 1);

            i++;
            if(!so.get_period(begin_time, current_time))
                continue;

            auto new_point = *so.points.rbegin();
            if (new_point.second.moving_avg == -1)
                continue;

            auto action = trade_strategy(new_point.second);
            switch(action){
                case do_nothing: break;
                case buy:
                    buy_stock(utility_t::tm_move_key(new_point.first, 1));
                    break;
                case sell:
                    sell_stock(utility_t::tm_move_key(new_point.first, 1));
                    break;
            }
        }
        sell_stock(so.points.rbegin()->first);
    }

    trading_t(const std::string &file_path, std::tm _time): so(file_path),
                current_time(utility_t::tm_to_key(_time)), points(points)
    {
        points = std::map<std::tm, point_t>{};
    }
};

#endif //STOCKS_TRADING_H
