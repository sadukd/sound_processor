#include "waveform.h"

double Waveform::durationSeconds() const
{
    return static_cast<double>(_samples.size()) / SampleRate;
}

size_t Waveform::samplesCount() const
{
    return _samples.size();
}

size_t Waveform::secondsToSamples(double sec) const
{
    return static_cast<size_t>(sec * SampleRate);
}

void Waveform::appendSample(int16_t sample)
{
    _samples.push_back(sample);
}

int16_t Waveform::getSample(size_t i) const
{
    return _samples.at(i);
}

void Waveform::setSample(size_t i, int16_t sample)
{
    _samples.at(i) = sample;
}

double Waveform::samplesToSeconds(size_t samples) const
{
    return static_cast<double>(samples) / SampleRate;
}

