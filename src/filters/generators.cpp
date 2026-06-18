#include "filters/generators.h"

#include "filters/filter_utils.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace {
constexpr double PI_VALUE = 3.14159265358979323846;

std::vector<int16_t> allocateSamples(double durationMs)
{
    size_t samplesCount = Waveform().millisecondsToSamples(durationMs);
    return std::vector<int16_t>(samplesCount);
}
}  // namespace

SineGenerator::SineGenerator(double frequencyHz, double durationMs)
    : _frequencyHz(frequencyHz), _durationMs(durationMs)
{
    if(durationMs < 0.0)
        throw std::invalid_argument(
            "Generator sin duration must be non-negative");

    if(frequencyHz < 0.0)
        throw std::invalid_argument(
            "Generator sin frequency must be non-negative");
}

void SineGenerator::apply(Waveform& sound)
{
    std::vector<int16_t> samples = allocateSamples(_durationMs);

    for(size_t i = 0; i < samples.size(); ++i)
    {
        double timeSec = static_cast<double>(i) / Waveform::SAMPLE_RATE;
        samples[i] = clampToSample(
            32767.0 * std::sin(2.0 * PI_VALUE * _frequencyHz * timeSec));
    }

    sound.replaceSamples(std::move(samples));
}

AmGenerator::AmGenerator(double amplitude, double carrierHz,
                         double modulationHz, double depth, double durationMs)
    : _amplitude(amplitude), _carrierHz(carrierHz), _modulationHz(modulationHz),
      _depth(depth), _durationMs(durationMs)
{
    if(durationMs < 0.0)
        throw std::invalid_argument(
            "Generator am duration must be non-negative");

    if(amplitude < 0.0 || amplitude > 1.0 || carrierHz < 0.0 ||
       modulationHz < 0.0 || depth < 0.0 || depth > 1.0)
    {
        throw std::invalid_argument("Generator am parameters are out of range");
    }
}

void AmGenerator::apply(Waveform& sound)
{
    std::vector<int16_t> samples = allocateSamples(_durationMs);

    for(size_t i = 0; i < samples.size(); ++i)
    {
        double timeSec = static_cast<double>(i) / Waveform::SAMPLE_RATE;
        double envelope =
            1.0 + _depth * std::sin(2.0 * PI_VALUE * _modulationHz * timeSec);
        double carrier = std::sin(2.0 * PI_VALUE * _carrierHz * timeSec);
        samples[i] = clampToSample(_amplitude * 32767.0 * envelope * carrier);
    }

    sound.replaceSamples(std::move(samples));
}

FmGenerator::FmGenerator(double amplitude, double carrierHz,
                         double modulationHz, double deviationHz,
                         double durationMs)
    : _amplitude(amplitude), _carrierHz(carrierHz), _modulationHz(modulationHz),
      _deviationHz(deviationHz), _durationMs(durationMs)
{
    if(durationMs < 0.0)
        throw std::invalid_argument(
            "Generator fm duration must be non-negative");

    if(amplitude < 0.0 || amplitude > 1.0 || carrierHz < 0.0 ||
       modulationHz <= 0.0 || deviationHz < 0.0)
    {
        throw std::invalid_argument("Generator fm parameters are out of range");
    }
}

void FmGenerator::apply(Waveform& sound)
{
    std::vector<int16_t> samples = allocateSamples(_durationMs);

    for(size_t i = 0; i < samples.size(); ++i)
    {
        double timeSec = static_cast<double>(i) / Waveform::SAMPLE_RATE;
        double phase = 2.0 * PI_VALUE * _carrierHz * timeSec +
                       (_deviationHz / _modulationHz) *
                           std::sin(2.0 * PI_VALUE * _modulationHz * timeSec);
        samples[i] = clampToSample(_amplitude * 32767.0 * std::sin(phase));
    }

    sound.replaceSamples(std::move(samples));
}
