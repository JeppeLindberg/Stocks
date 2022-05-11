#include <string>
#include "doctest.h"
#include "concurrent_trading.h"

TEST_CASE("concurrent_trading")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;
    trading_strategies_t ts{};

    SUBCASE("concurrent trading")
    {
        using namespace std::literals::string_literals;

        std::tm start_time{};
        utility_t::set_time("2011-11-01T01:00:00.000+0200"s, start_time);
        concurrent_trading_t ct{start_time, sample_dir + "JYSK.json", 365};

        ct.thread_prototypes.push_back(std::pair{"strategy_1", ts.strategy_1});
        ct.thread_prototypes.push_back(std::pair{"strategy_2", ts.strategy_2});

        ct.join();
    }
}