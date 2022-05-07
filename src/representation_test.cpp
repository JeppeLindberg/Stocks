
#include "doctest.h"
#include "representation.h"


TEST_CASE("representations")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;

    SUBCASE("candle")
    {
        representation_candle_t rc(resources_dir + "test_2.json", 3);

        rc.get_data();
        rc.get_data();


    }
}