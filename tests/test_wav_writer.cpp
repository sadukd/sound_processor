#include <catch2/catch_test_macros.hpp>

#include "wav_reader.h"
#include "wav_writer.h"
#include "waveform.h"

#include <cmath>
#include <filesystem>
#include <vector>

TEST_CASE("WavWriter writes valid wav file")
{
    std::filesystem::create_directories(
        "test_output"
    );

    std::vector<int16_t> input{
        100,
        -200,
        300,
        -400,
        500
    };

    Waveform original(input);

    WavWriter writer;

    writer.writer(
        "test_output/generated.wav",
        original
    );

    WavReader reader;

    Waveform loaded =
        reader.read(
            "test_output/generated.wav"
        );

    REQUIRE(
        loaded.samplesCount() ==
        original.samplesCount()
    );

    for (size_t i = 0;
         i < original.samplesCount();
         i++)
    {
        REQUIRE(
            loaded.getSample(i) ==
            original.getSample(i)
        );
    }
}

TEST_CASE("Generate 10 second sine wave")
{
    std::filesystem::create_directories(
        "test_output"
    );

    constexpr double frequency = 440.0; // A4
    constexpr double duration = 10.0;
    constexpr double amplitude = 30000.0;

    const size_t sampleCount =
        static_cast<size_t>(
            Waveform::SampleRate * duration
        );

    std::vector<int16_t> samples;
    samples.reserve(sampleCount);

    for (size_t i = 0; i < sampleCount; i++)
    {
        double t =
            static_cast<double>(i) /
            Waveform::SampleRate;

        double value =
            amplitude *
            std::sin(
                2.0 * M_PI * frequency * t
            );

        samples.push_back(
            static_cast<int16_t>(value)
        );
    }

    Waveform waveform(samples);

    WavWriter writer;

    writer.writer(
        "test_output/sine_440hz.wav",
        waveform
    );

    REQUIRE(
        std::filesystem::exists(
            "test_output/sine_440hz.wav"
        )
    );
}