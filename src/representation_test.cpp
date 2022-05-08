
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
        utility_t::set_time("2012-08-02T01:00:00.000+0200"s, start);
        utility_t::set_time("2012-08-06T01:00:00.000+0200"s, end);

        auto period = rc.get_period(start, end);
    }
}