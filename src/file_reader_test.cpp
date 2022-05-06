
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

        fr.read_trades(1, 0);
    }

    SUBCASE("read 1, 0")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");

        auto stock = fr.read_trades(1, 0);

        CHECK(stock == stock_json_t{"data1",
                                    "data2",
                                    20,
                                    "data3",
                                    "data4",
                                    "data5",
                                    "data6",
                                    "data7",
                                    std::vector<trade_json_t>{
                                        trade_json_t{
                                            "2012-10-31T09:00:01.000+0100",
                                            1,
                                            2,
                                            "trade1_1",
                                            "trade1_2",
                                            3,
                                            4}}});
    }

    SUBCASE("read 1, 1")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");

        auto stock = fr.read_trades(1, 1);

        CHECK(stock == stock_json_t{"data1",
                                    "data2",
                                    20,
                                    "data3",
                                    "data4",
                                    "data5",
                                    "data6",
                                    "data7",
                                    std::vector<trade_json_t>{
                                            trade_json_t{
                                                    "2012-10-31T09:01:07.000+0100",
                                                    5.5,
                                                    6,
                                                    "trade2_1",
                                                    "trade2_2",
                                                    7,
                                                    8}}});
    }

    SUBCASE("read 2, 0")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");

        auto stock = fr.read_trades(2, 0);

        CHECK(stock == stock_json_t{"data1",
                                    "data2",
                                    20,
                                    "data3",
                                    "data4",
                                    "data5",
                                    "data6",
                                    "data7",
                                    std::vector<trade_json_t>{
                                            trade_json_t{
                                                    "2012-10-31T09:00:01.000+0100",
                                                    1,
                                                    2,
                                                    "trade1_1",
                                                    "trade1_2",
                                                    3,
                                                    4},
                                            trade_json_t{
                                                    "2012-10-31T09:01:07.000+0100",
                                                    5.5,
                                                    6,
                                                    "trade2_1",
                                                    "trade2_2",
                                                    7,
                                                    8}}});
    }
}

