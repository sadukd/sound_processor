#include "waveform.h"

double Waveform::durationSeconds() const
{
    return static_cast<double>(_samples.size()) / SAMPLE_RATE;
}

size_t Waveform::samplesCount() const { return _samples.size(); }

size_t Waveform::secondsToSamples(double seconds) const
{
    return static_cast<size_t>(seconds * SAMPLE_RATE);
}

size_t Waveform::millisecondsToSamples(double milliseconds) const
{
    return secondsToSamples(milliseconds / 1000.0);
}

void Waveform::appendSample(int16_t sample) { _samples.push_back(sample); }

int16_t Waveform::getSample(size_t index) const { return _samples.at(index); }

void Waveform::setSample(size_t index, int16_t sample)
{
    _samples.at(index) = sample;
}

double Waveform::samplesToSeconds(size_t samples) const
{
    return static_cast<double>(samples) / SAMPLE_RATE;
}

double Waveform::samplesToMilliseconds(size_t samples) const
{
    return samplesToSeconds(samples) * 1000.0;
}

void Waveform::replaceSamples(std::vector<int16_t> samples)
{
    _samples = std::move(samples);
}

void Waveform::clear() { _samples.clear(); }

const std::vector<int16_t>& Waveform::getSamples() const { return _samples; }

std::vector<int16_t>& Waveform::getSamples() { return _samples; }
