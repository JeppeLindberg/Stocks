
#include "doctest.h"
#include "stock_file_reader.h"
#include <iostream>

TEST_CASE("read file")
{
    using namespace std::literals::string_literals;
    auto resources_dir = "C://Users//Jeppe//Documents//SP//Stocks//resources//"s;
    auto sample_dir = "C://Users//Jeppe//Downloads//SPexam_sample_input_data//json-2y//"s;

    SUBCASE("open file")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");
    }

    SUBCASE("read page 0")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");
        fr.page_size = 1;

        auto stock = fr.read_page(0);

        CHECK(stock == stock_t{"data1",
                               "data2",
                               20,
                               "data3",
                               "data4",
                               "data5",
                               "data6",
                               "data7",
                               std::set<trade_t>{
                                       trade_t{
                                            "2012-10-31T09:00:01.000+0100",
                                            1,
                                            2,
                                            "trade1_1",
                                            "trade1_2",
                                            3,
                                            4}}});
    }

    SUBCASE("read page 1")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");
        fr.page_size = 1;

        auto stock = fr.read_page(1);

        CHECK(stock == stock_t{"data1",
                               "data2",
                               20,
                               "data3",
                               "data4",
                               "data5",
                               "data6",
                               "data7",
                               std::set<trade_t>{
                                       trade_t{
                                                    "2012-10-31T09:01:07.000+0100",
                                                    5.5,
                                                    6,
                                                    "trade2_1",
                                                    "trade2_2",
                                                    7,
                                                    8}}});
    }

    SUBCASE("read page 0, size 2")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");
        fr.page_size = 2;

        auto stock = fr.read_page(0);

        CHECK(stock == stock_t{"data1",
                               "data2",
                               20,
                               "data3",
                               "data4",
                               "data5",
                               "data6",
                               "data7",
                               std::set<trade_t>{
                                       trade_t{
                                                    "2012-10-31T09:00:01.000+0100",
                                                    1,
                                                    2,
                                                    "trade1_1",
                                                    "trade1_2",
                                                    3,
                                                    4},
                                       trade_t{
                                                    "2012-10-31T09:01:07.000+0100",
                                                    5.5,
                                                    6,
                                                    "trade2_1",
                                                    "trade2_2",
                                                    7,
                                                    8}}});
    }

    SUBCASE("read page 0 and 1")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");
        fr.page_size = 1;

        fr.read_page(0);
        auto stock = fr.read_page(1);

        std::cout << stock << std::endl;

        CHECK(stock == stock_t{"data1","data2",20,"data3","data4","data5","data6","data7",
                               std::set<trade_t>{
                                       trade_t{
                                               "2012-10-31T09:00:01.000+0100",
                                               1,
                                               2,
                                               "trade1_1",
                                               "trade1_2",
                                               3,
                                               4},
                                       trade_t{
                                               "2012-10-31T09:01:07.000+0100",
                                               5.5,
                                               6,
                                               "trade2_1",
                                               "trade2_2",
                                               7,
                                               8}}});
    }

    SUBCASE("read page 0 and 0, no duplicates")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");
        fr.page_size = 1;

        fr.read_page(0);
        auto stock = fr.read_page(0);

        std::cout << stock << std::endl;

        CHECK(stock == stock_t{"data1","data2",20,"data3","data4","data5","data6","data7",
                               std::set<trade_t>{
                                       trade_t{
                                               "2012-10-31T09:00:01.000+0100",
                                               1,
                                               2,
                                               "trade1_1",
                                               "trade1_2",
                                               3,
                                               4}}});
    }

    SUBCASE("read first trade")
    {
        stock_file_reader_t fr(resources_dir + "test_1.json");

        auto stock = fr.read_first_trade();

        CHECK(stock == stock_t{"data1","data2",20,"data3","data4","data5","data6","data7",
                               std::set<trade_t>{
                                       trade_t{
                                               "2012-10-31T09:00:01.000+0100",
                                               1,
                                               2,
                                               "trade1_1",
                                               "trade1_2",
                                               3,
                                               4}}});
    }


    /*SUBCASE("read sample")
    {
        stock_file_reader_t fr(sample_dir + "DSV.json");

        auto stock = fr.read_trades(1000, 30000);

        std::cout << stock << std::endl;
    }*/

}

