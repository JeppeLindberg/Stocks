#include "doctest.h"
#include "stochastic_oscillator.h"


TEST_CASE("stochastic_oscillator")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;

    SUBCASE("stochastic test_2 lowest_overall so closing_price highest_overall values")
    {
        using namespace std::literals::string_literals;
        stochastic_oscillator_t so(resources_dir + "test_2.json");

        std::tm start{};
        std::tm end{};
        utility_t::set_time("2012-01-02T01:00:01.000+0100"s, start);
        utility_t::set_time("2012-01-21T01:00:01.000+0100"s, end);

        auto points = so.get_period(start, end);

        std::tm target{};
        utility_t::set_time("2012-01-16T01:00:01.000+0100"s, target);
        target = utility_t::tm_to_key(target);

        CHECK(points[target].lowest_overall == 1);
        target = utility_t::tm_move_key(target, 1);
        CHECK(points[target].lowest_overall == 1);
        target = utility_t::tm_move_key(target, 1);
        CHECK(points[target].so == 1);
        CHECK(points[target].closing_price == 7);
        target = utility_t::tm_move_key(target, 1);
        CHECK(points[target].closing_price == 10);
        CHECK(points[target].highest_overall == 10);
        target = utility_t::tm_move_key(target, 1);
        CHECK(points[target].closing_price == 9);
        CHECK(points[target].highest_overall == 10);
    }

    /*SUBCASE("stochastic BAVA")
    {
        using namespace std::literals::string_literals;
        stochastic_oscillator_t so(sample_dir + "BAVA.json");

        std::tm start{};
        std::tm end{};
        utility_t::set_time("2011-00-01T01:00:00.000+0200"s, start);
        utility_t::set_time("2012-00-01T01:00:00.000+0200"s, end);

        auto points = so.get_period(start, end);
    }*/
}