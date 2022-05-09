#include "trading.h"
#include "doctest.h"


TEST_CASE("trading test")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;

    SUBCASE("trading BAVA")
    {
        using namespace std::literals::string_literals;
        std::tm start{};
        utility_t::set_time("2012-02-01T01:00:00.000+0200"s, start);

        trading_t trading{sample_dir + "BAVA.json", start};

        trading.simulate_trades(20);
    }
}
