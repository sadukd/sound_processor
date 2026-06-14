#include <catch2/catch_test_macros.hpp>

#include "wav_reader.h"
#include "wav_writer.h"
#include "waveform.h"

#include <cmath>
#include <filesystem>
#include <vector>

TEST_CASE("wav writer writes readable file")
{
    std::filesystem::create_directories("test_output");

    Waveform original({100, -200, 300, -400, 500});
    WavWriter writer;
    writer.write("test_output/generated.wav", original);

    WavReader reader;
    Waveform loaded = reader.read("test_output/generated.wav");

    REQUIRE(loaded.samplesCount() == original.samplesCount());

    for(size_t i = 0; i < original.samplesCount(); ++i)
        REQUIRE(loaded.getSample(i) == original.getSample(i));
}

TEST_CASE("wav writer writes empty waveform")
{
    std::filesystem::create_directories("test_output");

    Waveform empty;
    WavWriter writer;
    writer.write("test_output/empty_generated.wav", empty);

    WavReader reader;
    Waveform loaded = reader.read("test_output/empty_generated.wav");

    REQUIRE(loaded.samplesCount() == 0);
}

TEST_CASE("wav writer can persist generated sine")
{
    std::filesystem::create_directories("test_output");

    constexpr double FREQUENCY = 440.0;
    constexpr double DURATION_SECONDS = 1.0;
    constexpr double AMPLITUDE = 30000.0;

    const auto SAMPLE_COUNT =
        static_cast<size_t>(Waveform::SAMPLE_RATE * DURATION_SECONDS);

    std::vector<int16_t> samples;
    samples.reserve(SAMPLE_COUNT);

    for(size_t i = 0; i < SAMPLE_COUNT; ++i)
    {
        double timeSec = static_cast<double>(i) / Waveform::SAMPLE_RATE;
        samples.push_back(static_cast<int16_t>(
            AMPLITUDE *
            std::sin(2.0 * 3.14159265358979323846 * FREQUENCY * timeSec)));
    }

    WavWriter writer;
    writer.write("test_output/sine_440hz.wav", Waveform(samples));

    REQUIRE(std::filesystem::exists("test_output/sine_440hz.wav"));
}
