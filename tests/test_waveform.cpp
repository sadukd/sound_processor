#include "waveform.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;
TEST_CASE("duration and samples count")
{
    Waveform w;

    for(int i = 0; i < 44100; i++)
        w.appendSample(100);

    REQUIRE(w.samplesCount() == 44100);
    REQUIRE(w.durationSeconds() == Approx(1.0));
}

TEST_CASE("seconds to samples conversion")
{
    Waveform w;

    REQUIRE(w.secondsToSamples(1.0) == 44100);
    REQUIRE(w.secondsToSamples(0.5) == 22050);
    REQUIRE(w.secondsToSamples(2.0) == 88200);
}

TEST_CASE("sample read write")
{
    Waveform w;

    w.appendSample(10);
    w.appendSample(20);
    w.appendSample(30);

    REQUIRE(w.getSample(0) == 10);
    REQUIRE(w.getSample(1) == 20);
    REQUIRE(w.getSample(2) == 30);

    w.setSample(1, 999);

    REQUIRE(w.getSample(1) == 999);
}

TEST_CASE("out of range access throws")
{
    Waveform w;

    w.appendSample(1);

    REQUIRE_THROWS_AS(w.getSample(10), std::out_of_range);

    REQUIRE_THROWS_AS(w.setSample(10, 5), std::out_of_range);
}

TEST_CASE("empty waveform")
{
    Waveform w;

    REQUIRE(w.samplesCount() == 0);
    REQUIRE(w.durationSeconds() == Approx(0.0));
}