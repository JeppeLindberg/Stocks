
#include "doctest.h"
#include "json.h"
#include <sstream>
#include <vector>

struct aggregate_t
{ /** public access is just for easy structured initialization in tests */
    bool b;
    int x;
    double y;
    std::string z;
    std::vector<int> w;

    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        visit("b", b);
        visit("x", x);
        visit("y", y);
        visit("z", z);
        visit("w", w);
    }

    friend bool operator==(const aggregate_t& a1, const aggregate_t& a2)
    {
        return (a1.b == a2.b) && (a1.x == a2.x) && (a1.y == a2.y) && (a1.z == a2.z) && (a1.w == a2.w);
    }
};


/** custom class to test JSON input/output with nesting */
struct nested_t
{
    std::string text;
    aggregate_t agg;

    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        visit("text", text);
        visit("agg", agg);
    }

    friend bool operator==(const nested_t& n1, const nested_t& n2)
    {
        return (n1.text == n2.text) && (n1.agg == n2.agg);
    }
};

TEST_CASE("JSON input")
{
    SUBCASE("boolean: true")
    {
        auto is = std::istringstream{"true"};
        auto v = false;
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == true);
    }
    SUBCASE("boolean: false")
    {
        auto is = std::istringstream{"false"};
        auto v = true;
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == false);
    }
    SUBCASE("integer")
    {
        auto is = std::istringstream{"7"};
        auto v = 0;
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == 7);
    }
    SUBCASE("double")
    {
        auto is = std::istringstream{"3.14"};
        auto v = 0.0;
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == 3.14);
    }
    SUBCASE("cpp-string")
    {
        auto is = std::istringstream{"\"hello\""};
        auto v = std::string{};
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(v == "hello");
    }
    SUBCASE("container")
    {
        auto is = std::istringstream{"[3,7,11]"};
        auto v = std::vector<int>{};
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == std::vector{3, 7, 11});
    }
    SUBCASE("tuple")
    {
        using namespace std::literals::string_literals;
        auto is = std::istringstream{R"({"1":7,"2":3.14,"3":"hello"})"};
        auto v = std::tuple{13, 2.71, "bye"s};
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(v == std::tuple{7, 3.14, "hello"});
    }
    SUBCASE("aggregate")
    {
        auto is = std::istringstream{R"({"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]})"};
        auto v = aggregate_t{};
        static_assert(accepts_writer_v<aggregate_t, json_writer_t>, "does not accept writer");
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == aggregate_t{true, 3, 3.14, "hello", {7, 11}});
    }
    SUBCASE("nested")
    {
        auto is =
                std::istringstream{R"({"text":"complicated","agg":{"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]}})"};
        auto v = nested_t{};
        static_assert(accepts_writer_v<nested_t, json_writer_t>, "does not accept writer");
        auto amount = -1;
        auto offset = -1;
        is >> json_t{v, amount, offset};
        CHECK(is);
        CHECK(v == nested_t{"complicated", {true, 3, 3.14, "hello", {7, 11}}});
    }
}
