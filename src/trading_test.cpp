#include "trading.h"
#include "doctest.h"

TEST_CASE("trading test")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;
    trading_strategies_t ts{};

    SUBCASE("trading test_2")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        set_time("2012-01-21T01:00:00.000+0100"s, start);

        trading_t trading{resources_dir + "test_2.json", start};
        trading.trade_strategy = ts.strategy_2;

        trading.simulate_trades(11);
        CHECK(trading.result_as_string() == "Buy at 4 (2012-1-26T0:0:0)\nSell at 14 (2012-2-1T0:0:0)\n");
        CHECK(trading.money == 35000);
    }

    SUBCASE("trading JYSK")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        set_time("2011-11-01T01:00:00.000+0200"s, start);

        trading_t trading{sample_dir + "JYSK.json", start};
        trading.trade_strategy = ts.strategy_2;

        trading.simulate_trades(365);
        std::cout << trading.result_as_string() << std::endl;
        std::cout << trading.money << std::endl;
    }

    SUBCASE("trading JYSK benchmark")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        set_time("2011-11-01T01:00:00.000+0200"s, start);

        trading_t trading{sample_dir + "JYSK.json", start};
        trading.trade_strategy = ts.strategy_2;

        trading.benchmark = true;
        trading.simulate_trades(365);
    }

    SUBCASE("trading test_2 lambda")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        set_time("2012-01-21T01:00:00.000+0100"s, start);

        trading_t trading{resources_dir + "test_2.json", start};
        trading.trade_strategy = [](point_t point)
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

        trading.simulate_trades(11);
        CHECK(trading.result_as_string() == "Buy at 4 (2012-1-26T0:0:0)\nSell at 14 (2012-2-1T0:0:0)\n");
        CHECK(trading.money == 35000);
    }
    
    SUBCASE("trading GEN")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        set_time("2011-11-01T01:00:00.000+0200"s, start);

        trading_t trading{sample_dir + "GEN.json", start};
        trading.trade_strategy = ts.strategy_2;

        trading.simulate_trades(365);
        std::cout << trading.result_as_string() << std::endl;
        std::cout << trading.money << std::endl;
    }
}
