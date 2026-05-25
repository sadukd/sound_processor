#include <catch2/catch_test_macros.hpp>
#include "parser.h"

TEST_CASE("Parser basic input output filter parsing")
{
    ArgsParser parser;

    char* argv[] = {
        (char*)"prog",
        (char*)"-i",
        (char*)"input.txt",
        (char*)"-o",
        (char*)"output.txt",
        (char*)"-f",
        (char*)"blur",
        (char*)"5",
        (char*)"10"
    };

    auto res = parser.parse(9, argv);

    REQUIRE(res == Result::ok);
    REQUIRE(parser.getInFileName() == "input.txt");
    REQUIRE(parser.getOutFileName() == "output.txt");

    auto filters = parser.getFilters();
    REQUIRE(filters.size() == 1);
    REQUIRE(filters[0].filterName == "blur");
    REQUIRE(filters[0].params.size() == 2);
    REQUIRE(filters[0].params[0] == "5");
    REQUIRE(filters[0].params[1] == "10");
}