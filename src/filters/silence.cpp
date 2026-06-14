#include "filters/silence.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

Silence::Silence(const std::string& unit, double start, double end)
    : _unit(unit), _start(start), _end(end)
{
    if(unit != "sec" && unit != "ms")
        throw std::invalid_argument("Silence unit must be sec or ms");

    if(start < 0.0)
        throw std::invalid_argument("Silence start must be >= 0");

    if(end < start)
        throw std::invalid_argument("Silence end must be >= start");
}

size_t Silence::timeToSamples(double value) const
{
    if(_unit == "ms")
        return Waveform().millisecondsToSamples(value);

    return Waveform().secondsToSamples(value);
}

void Silence::apply(Waveform& sound)
{
    size_t startSample = timeToSamples(_start);
    size_t endSample = timeToSamples(_end);

    size_t insertCount = endSample - startSample + 1;
    size_t insertPos = std::min(startSample, sound.samplesCount());

    auto& samples = sound.getSamples();
    samples.insert(samples.begin() + insertPos, insertCount, 0);
}
