#include <catch2/catch_test_macros.hpp>

#include "application.h"
#include "wav_reader.h"
#include "wav_writer.h"

#include <filesystem>

TEST_CASE("application writes empty file when only output is given")
{
    std::filesystem::create_directories("test_output");

    Application application;
    application.configure();

    char program[] = "sound_processor";
    char outFlag[] = "-o";
    char outPath[] = "test_output/app_empty.wav";
    char* argv[] = {program, outFlag, outPath};

    REQUIRE(application.start(3, argv) == 0);
    REQUIRE(std::filesystem::exists("test_output/app_empty.wav"));

    WavReader reader;
    REQUIRE(reader.read("test_output/app_empty.wav").samplesCount() == 0);
}

TEST_CASE("application applies generator pipeline")
{
    std::filesystem::create_directories("test_output");

    Application application;
    application.configure();

    char program[] = "sound_processor";
    char outFlag[] = "-o";
    char outPath[] = "test_output/app_generated.wav";
    char filterFlag[] = "-f";
    char generator[] = "generator";
    char mode[] = "sin";
    char frequency[] = "440";
    char duration[] = "100";
    char* argv[] = {program,   outFlag, outPath,   filterFlag,
                    generator, mode,    frequency, duration};

    REQUIRE(application.start(8, argv) == 0);

    WavReader reader;
    REQUIRE(reader.read("test_output/app_generated.wav").samplesCount() ==
            Waveform().millisecondsToSamples(100.0));
}
