#include <catch2/catch_test_macros.hpp>

#include "parser.h"

#include <string_view>

static char* cstr(const char* text) { return const_cast<char*>(text); }

TEST_CASE("no args returns help mode")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor")};

    REQUIRE(parser.parse(1, argv) == Result::noArgs);
}

TEST_CASE("parser accepts output only")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"), cstr("-o"), cstr("out.wav")};

    REQUIRE(parser.parse(3, argv) == Result::ok);
    REQUIRE_FALSE(parser.getInFileName().has_value());
    REQUIRE(parser.getOutFileName().has_value());
    REQUIRE(*parser.getOutFileName() == "out.wav");
    REQUIRE(parser.getFilters().empty());
}

TEST_CASE("parser accepts input output and repeated filters")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"),
                    cstr("-i"),
                    cstr("in.wav"),
                    cstr("-o"),
                    cstr("out.wav"),
                    cstr("-f"),
                    cstr("ampl"),
                    cstr("0.8"),
                    cstr("-f"),
                    cstr("silence"),
                    cstr("sec"),
                    cstr("0.2"),
                    cstr("0.4")};

    REQUIRE(parser.parse(13, argv) == Result::ok);
    REQUIRE(parser.getInFileName().has_value());
    REQUIRE(*parser.getInFileName() == "in.wav");
    REQUIRE(parser.getOutFileName().has_value());
    REQUIRE(*parser.getOutFileName() == "out.wav");
    REQUIRE(parser.getFilters().size() == 2);
    REQUIRE(parser.getFilters()[0].filterName == "ampl");
    REQUIRE(parser.getFilters()[0].params ==
            std::vector<std::string_view>{"0.8"});
    REQUIRE(parser.getFilters()[1].filterName == "silence");
    REQUIRE(parser.getFilters()[1].params ==
            std::vector<std::string_view>{"sec", "0.2", "0.4"});
}

TEST_CASE("parser accepts filters without input or output")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"),
                    cstr("-f"),
                    cstr("generator"),
                    cstr("sin"),
                    cstr("440"),
                    cstr("1000")};

    REQUIRE(parser.parse(6, argv) == Result::ok);
    REQUIRE_FALSE(parser.getInFileName().has_value());
    REQUIRE_FALSE(parser.getOutFileName().has_value());
    REQUIRE(parser.getFilters().size() == 1);
}

TEST_CASE("parser rejects duplicate input")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"), cstr("-i"), cstr("a.wav"),
                    cstr("-i"), cstr("b.wav")};

    REQUIRE(parser.parse(5, argv) == Result::badArgs);
}

TEST_CASE("parser rejects missing flag value")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"), cstr("-o")};

    REQUIRE(parser.parse(2, argv) == Result::badArgs);
}

TEST_CASE("parser rejects empty filter block")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"), cstr("-f")};

    REQUIRE(parser.parse(2, argv) == Result::badArgs);
}

TEST_CASE("parser rejects stray token")
{
    ArgsParser parser;
    char* argv[] = {cstr("sound_processor"), cstr("random.wav")};

    REQUIRE(parser.parse(2, argv) == Result::badArgs);
}
