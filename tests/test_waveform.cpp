#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "waveform.h"

using Catch::Approx;

TEST_CASE("waveform duration and count")
{
    Waveform waveform;

    for(int i = 0; i < 44100; ++i)
        waveform.appendSample(100);

    REQUIRE(waveform.samplesCount() == 44100);
    REQUIRE(waveform.durationSeconds() == Approx(1.0));
}

TEST_CASE("waveform sample time conversions")
{
    Waveform waveform;

    REQUIRE(waveform.secondsToSamples(1.0) == 44100);
    REQUIRE(waveform.millisecondsToSamples(500.0) == 22050);
    REQUIRE(waveform.samplesToSeconds(22050) == Approx(0.5));
    REQUIRE(waveform.samplesToMilliseconds(4410) == Approx(100.0));
}

TEST_CASE("waveform read write and replace")
{
    Waveform waveform;
    waveform.appendSample(10);
    waveform.appendSample(20);
    waveform.appendSample(30);

    REQUIRE(waveform.getSample(1) == 20);

    waveform.setSample(1, 999);
    REQUIRE(waveform.getSample(1) == 999);

    waveform.replaceSamples({1, 2, 3, 4});
    REQUIRE(waveform.samplesCount() == 4);
    REQUIRE(waveform.getSample(3) == 4);

    waveform.clear();
    REQUIRE(waveform.samplesCount() == 0);
}

TEST_CASE("waveform range checks")
{
    Waveform waveform;
    waveform.appendSample(1);

    REQUIRE_THROWS_AS(waveform.getSample(10), std::out_of_range);
    REQUIRE_THROWS_AS(waveform.setSample(10, 5), std::out_of_range);
}
