#pragma once

#include <cstdint>
#include <vector>

class Waveform
{
public:
    static uint16_t constexpr SampleRate = 44100;

    static uint16_t constexpr SampleSize = 16; // bits

    static uint16_t constexpr AudioFormat = 1; // PCM

    static uint16_t constexpr ChannelAmnt = 1; // Mono

    double durationSeconds() const;

    size_t samplesCount() const;

    size_t secondsToSamples(double sec) const;

    void appendSample(int16_t sample);

    int16_t getSample(size_t i) const;

    void setSample(size_t i, int16_t sample);

    double samplesToSeconds(size_t samples) const;

private:
    std::vector<int16_t> _samples;
};