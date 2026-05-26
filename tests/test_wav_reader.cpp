#include "wav_reader.h"
#include "waveform.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <fstream>
#include <vector>

static void writeWav16Mono(const std::string& path,
                           const std::vector<int16_t>& samples)
{
    std::ofstream f(path, std::ios::binary);

    uint32_t dataSize = samples.size() * sizeof(int16_t);
    uint32_t fmtSize = 16;
    uint32_t riffSize = 4 + (8 + fmtSize) + (8 + dataSize);

    f.write("RIFF", 4);
    f.write(reinterpret_cast<char*>(&riffSize), 4);
    f.write("WAVE", 4);

    f.write("fmt ", 4);
    f.write(reinterpret_cast<char*>(&fmtSize), 4);

    uint16_t audioFormat = 1;
    uint16_t channels = 1;
    uint32_t sampleRate = 44100;
    uint32_t byteRate = sampleRate * channels * sizeof(int16_t);
    uint16_t blockAlign = channels * sizeof(int16_t);
    uint16_t bitsPerSample = 16;

    f.write(reinterpret_cast<char*>(&audioFormat), 2);
    f.write(reinterpret_cast<char*>(&channels), 2);
    f.write(reinterpret_cast<char*>(&sampleRate), 4);
    f.write(reinterpret_cast<char*>(&byteRate), 4);
    f.write(reinterpret_cast<char*>(&blockAlign), 2);
    f.write(reinterpret_cast<char*>(&bitsPerSample), 2);

    f.write("data", 4);
    f.write(reinterpret_cast<char*>(&dataSize), 4);
    f.write(reinterpret_cast<const char*>(samples.data()), dataSize);
}

TEST_CASE("WavReader reads valid mono PCM16 file")
{
    std::string path = "test.wav";

    std::vector<int16_t> input = {100, -100, 300, -300, 0};
    writeWav16Mono(path, input);

    WavReader r;
    Waveform w = r.read(path);

    REQUIRE(w.samplesCount() == input.size());
}

TEST_CASE("WavReader rejects non-existent file")
{
    WavReader r;

    REQUIRE_THROWS(r.read("no_file.wav"));
}

TEST_CASE("WavReader rejects invalid RIFF")
{
    std::ofstream f("bad.wav", std::ios::binary);
    f.write("XXXX", 4);

    WavReader r;

    REQUIRE_THROWS(r.read("bad.wav"));
}

TEST_CASE("WavReader rejects non-WAVE format")
{
    std::ofstream f("bad2.wav", std::ios::binary);

    uint32_t size = 0;
    f.write("RIFF", 4);
    f.write(reinterpret_cast<char*>(&size), 4);
    f.write("BEEF", 4);

    WavReader r;

    REQUIRE_THROWS(r.read("bad2.wav"));
}

TEST_CASE("WavReader handles empty data chunk")
{
    std::string path = "empty.wav";

    std::vector<int16_t> input = {};
    writeWav16Mono(path, input);

    WavReader r;
    Waveform w = r.read(path);

    REQUIRE(w.samplesCount() == 0);
}