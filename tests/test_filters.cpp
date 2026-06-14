#include <catch2/catch_test_macros.hpp>

#include "filters/ampl.h"
#include "filters/generators.h"
#include "filters/lowpass.h"
#include "filters/normalize.h"
#include "filters/silence.h"
#include "filters/timestretch.h"

TEST_CASE("ampl clamps overflow")
{
    Waveform sound({30000, -30000});
    Ampl filter(2.0);

    filter.apply(sound);

    REQUIRE(sound.getSample(0) == 32767);
    REQUIRE(sound.getSample(1) == -32768);
}

TEST_CASE("normalize uses default peak")
{
    Waveform sound({1000, -2000, 4000});
    Normalize filter;

    filter.apply(sound);

    REQUIRE(sound.getSample(2) == 32767);
}

TEST_CASE("silence inserts zeros into signal")
{
    Waveform sound({1, 2, 3, 4});
    Silence filter("sec", 0.0, 2.0 / Waveform::SAMPLE_RATE);

    filter.apply(sound);

    REQUIRE(sound.samplesCount() == 7);
    REQUIRE(sound.getSample(0) == 0);
    REQUIRE(sound.getSample(1) == 0);
    REQUIRE(sound.getSample(2) == 0);
    REQUIRE(sound.getSample(3) == 1);
}

TEST_CASE("timestretch doubles signal with interpolation")
{
    Waveform sound({0, 10, 20});
    TimeStretch filter(2.0);

    filter.apply(sound);

    REQUIRE(sound.samplesCount() == 6);
    REQUIRE(sound.getSample(0) == 0);
    REQUIRE(sound.getSample(1) == 5);
    REQUIRE(sound.getSample(2) == 10);
    REQUIRE(sound.getSample(3) == 15);
    REQUIRE(sound.getSample(5) == 20);
}

TEST_CASE("lowpass smooths with edge replication")
{
    Waveform sound({0, 0, 30, 0, 0});
    Lowpass filter(3);

    filter.apply(sound);

    REQUIRE(sound.getSample(0) == 0);
    REQUIRE(sound.getSample(1) == 10);
    REQUIRE(sound.getSample(2) == 10);
    REQUIRE(sound.getSample(3) == 10);
}

TEST_CASE("sine generator replaces input")
{
    Waveform sound({1, 2, 3});
    SineGenerator generator(440.0, 1000.0);

    generator.apply(sound);

    REQUIRE(sound.samplesCount() == Waveform().millisecondsToSamples(1000.0));
    REQUIRE(sound.getSample(0) == 0);
}

TEST_CASE("am generator creates expected sample count")
{
    Waveform sound;
    AmGenerator generator(0.5, 440.0, 4.0, 0.8, 250.0);

    generator.apply(sound);

    REQUIRE(sound.samplesCount() == Waveform().millisecondsToSamples(250.0));
}

TEST_CASE("fm generator creates expected sample count")
{
    Waveform sound;
    FmGenerator generator(0.5, 440.0, 5.0, 100.0, 250.0);

    generator.apply(sound);

    REQUIRE(sound.samplesCount() == Waveform().millisecondsToSamples(250.0));
}
