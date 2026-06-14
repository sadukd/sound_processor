#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <cstdint>
#include <vector>

class Waveform
{
public:
    Waveform(): _samples() {}
    Waveform(std::vector<int16_t> samples): _samples(std::move(samples)) {}

    static uint32_t constexpr SAMPLE_RATE = 44100;
    static uint16_t constexpr SAMPLE_SIZE = 16;   // bits
    static uint16_t constexpr AUDIO_FORMAT = 1;   // PCM
    static uint16_t constexpr CHANNEL_COUNT = 1;  // Mono

    double durationSeconds() const;

    size_t samplesCount() const;

    size_t secondsToSamples(double seconds) const;
    size_t millisecondsToSamples(double milliseconds) const;

    void appendSample(int16_t sample);

    int16_t getSample(size_t index) const;

    void setSample(size_t index, int16_t sample);

    double samplesToSeconds(size_t samples) const;
    double samplesToMilliseconds(size_t samples) const;

    void replaceSamples(std::vector<int16_t> samples);
    void clear();

    const std::vector<int16_t>& getSamples() const;

    std::vector<int16_t>& getSamples();

private:
    std::vector<int16_t> _samples;
};

#endif
