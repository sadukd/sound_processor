#include <catch2/catch_test_macros.hpp>
#include "parser.h"

static char* cstr(const char* s) { return const_cast<char*>(s); }

TEST_CASE("empty args")
{
    ArgsParser p;
    char* argv[] = { cstr("prog") };

    REQUIRE(p.parse(1, argv) != Result::ok);
}

TEST_CASE("basic input output")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-i"), cstr("in.txt"),
        cstr("-o"), cstr("out.txt")
    };

    REQUIRE(p.parse(5, argv) == Result::ok);
    REQUIRE(p.getInFileName() == "in.txt");
    REQUIRE(p.getOutFileName() == "out.txt");
}

TEST_CASE("input given twice should fail")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-i"), cstr("a.txt"),
        cstr("-i"), cstr("b.txt")
    };

    REQUIRE(p.parse(5, argv) == Result::badArgs);
}

TEST_CASE("output given twice should fail")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-o"), cstr("a.txt"),
        cstr("-o"), cstr("b.txt")
    };

    REQUIRE(p.parse(5, argv) == Result::badArgs);
}

TEST_CASE("unknown argument should fail")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("random.txt")
    };

    REQUIRE(p.parse(2, argv) == Result::badArgs);
}

TEST_CASE("filter with name and params")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-f"),
        cstr("blur"),
        cstr("5"),
        cstr("10")
    };

    REQUIRE(p.parse(5, argv) == Result::ok);

    auto filters = p.getFilters();
    REQUIRE(filters.size() == 1);
    REQUIRE(filters[0].filterName == "blur");
    REQUIRE(filters[0].params.size() == 2);
    REQUIRE(filters[0].params[0] == "5");
    REQUIRE(filters[0].params[1] == "10");
}

TEST_CASE("filter without name should fail")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-f"),
        cstr("5")
    };

    REQUIRE(p.parse(3, argv) == Result::badArgs);
}

TEST_CASE("mix input output filter full scenario")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-i"), cstr("in.txt"),
        cstr("-o"), cstr("out.txt"),
        cstr("-f"), cstr("blur"), cstr("5"), cstr("10"),
        cstr("-f"), cstr("sharpen"), cstr("2")
    };

    REQUIRE(p.parse(10, argv) == Result::ok);

    REQUIRE(p.getInFileName() == "in.txt");
    REQUIRE(p.getOutFileName() == "out.txt");

    auto filters = p.getFilters();
    REQUIRE(filters.size() == 2);

    REQUIRE(filters[0].filterName == "blur");
    REQUIRE(filters[0].params.size() == 2);

    REQUIRE(filters[1].filterName == "sharpen");
    REQUIRE(filters[1].params.size() == 1);
}

TEST_CASE("flag without value should fail")
{
    ArgsParser p;

    char* argv[] = {
        cstr("prog"),
        cstr("-i")
    };

    REQUIRE(p.parse(2, argv) == Result::badArgs);
}