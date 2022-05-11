
#include "doctest.h"
#include "representation.h"


TEST_CASE("representations")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;

    SUBCASE("candle read period")
    {
        using namespace std::literals::string_literals;
        representation_candle_t rc(sample_dir + "BAVA.json");

        std::tm start{};
        std::tm end{};
        set_time("2012-08-02T01:00:00.000+0200"s, start);
        set_time("2012-08-06T01:00:00.000+0200"s, end);

        auto period = rc.get_period(start, end);

        std::tm target = period.begin()->first;
        CHECK(period[target].first_price == 53);
        CHECK(period[target].last_price == 52);
        CHECK(period[target].min_price == 52);
        CHECK(period[target].max_price == 55);
        target = utility_t::tm_move_key(target, 1);
        CHECK(period[target].first_price == 52.5);
        CHECK(period[target].last_price == 53);
        CHECK(period[target].min_price == 51.5);
        CHECK(period[target].max_price == 53);
    }
}