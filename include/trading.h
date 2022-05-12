#ifndef STOCKS_TRADING_H
#define STOCKS_TRADING_H

#include <functional>
#include "stochastic_oscillator.h"
#include "benchmark.h"

enum Action{
    do_nothing = 0,
    buy = 1,
    sell = 2,
};

struct trading_strategies_t{
    const std::function<Action(point_t)> strategy_1 = [](point_t point)
    {
        if(point.moving_avg < 0.4 &&
           point.so > 0.4 &&
           std::abs(point.so - point.moving_avg) > 0.02)
            return Action{buy};
        if(point.moving_avg > 0.6 &&
           point.so < 0.6)
            return Action{sell};
        return Action{do_nothing};
    };
    const std::function<Action(point_t)> strategy_2 = [](point_t point)
    {
        if(point.moving_avg < 0.2 &&
           point.so > 0.2 &&
           std::abs(point.so - point.moving_avg) > 0.02)
            return Action{buy};
        if(point.moving_avg > 0.6 &&
           point.so < 0.6)
            return Action{sell};
        return Action{do_nothing};
    };
};

// Requirement 5, requirement 6
class trading_t{
    std::tm begin_time;

    void buy_stock(const std::tm& time){
        double price = so.get_opening_price_after(time);
        if(money > 0)
            sstream << "Buy at " << price << " (" << time << ")\n";
        shares += money/price;
        money = 0;
    }

    void sell_stock(const std::tm& time){
        double price = so.get_opening_price_after(time);
        if(shares > 0)
            sstream << "Sell at " << price << " (" << time << ")\n";
        money += shares*price;
        shares = 0;
    }

public:
    stochastic_oscillator_t so;
    std::tm current_time;
    std::function<Action(point_t)> trade_strategy;
    std::stringstream sstream;

    double money = 10000;
    double shares = 0;
    bool benchmark = false;

    void simulate_trades(int duration_days) {
        benchmark_t b{};

        begin_time = utility_t::tm_move_key(current_time, -20);
        int i = 0;
        while (i<duration_days){
            current_time = utility_t::tm_move_key(current_time, 1);

            i++;
            if(!so.get_period(begin_time, current_time))
                continue;
            // Possible point of optimization: Calculate all periods first, before simulating trades

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

        if (benchmark)
            std::cout << "trading_t.simulate_trades() benchmark: " << b.get_time() << " ms" << std::endl;
    }

    std::string result_as_string() {
        return sstream.str();
    }

    trading_t(const std::string &file_path, std::tm _time): so(file_path),
                current_time(utility_t::tm_to_key(_time)), sstream{}
    {}
};

#endif //STOCKS_TRADING_H
