#include "trading.h"
#include "doctest.h"

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

TEST_CASE("trading test")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;
    trading_strategies_t ts{};

    SUBCASE("trading JYSK")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        utility_t::set_time("2011-11-01T01:00:00.000+0200"s, start);

        trading_t trading{sample_dir + "JYSK.json", start};
        trading.trade_strategy = ts.strategy_2;

        trading.simulate_trades(365);
        std::cout << trading.money << std::endl;
    }

    /*SUBCASE("trading GEN")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        utility_t::set_time("2011-02-01T01:00:00.000+0200"s, start);

        trading_t trading{sample_dir + "GEN.json", start};
        trading.trade_strategy = ts.strategy_1;

        trading.simulate_trades(365);
        std::cout << trading.money << std::endl;
    }*/
}
