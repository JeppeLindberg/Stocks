
#include "doctest.h"
#include "stock_file_reader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

TEST_CASE("read file")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;

    SUBCASE("open file")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");

        fr.read_trades(0, 1000);
    }
}

