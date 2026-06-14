#include "filters/timestretch.h"

#include "filters/filter_utils.h"

#include <cmath>
#include <stdexcept>
#include <vector>

TimeStretch::TimeStretch(double factor): _factor(factor)
{
    if(factor <= 0.0)
        throw std::invalid_argument("Timestretch factor must be positive");
}

void TimeStretch::apply(Waveform& sound)
{
    if(sound.samplesCount() == 0)
        return;

    auto newSize =
        static_cast<size_t>(std::llround(sound.samplesCount() * _factor));
    std::vector<int16_t> stretched;
    stretched.reserve(newSize);

    for(size_t i = 0; i < newSize; ++i)
    {
        double pos = static_cast<double>(i) / _factor;
        auto left = static_cast<size_t>(std::floor(pos));
        double frac = pos - static_cast<double>(left);

        if(left >= sound.samplesCount() - 1)
        {
            stretched.push_back(sound.getSample(sound.samplesCount() - 1));
            continue;
        }

        double sample =
            static_cast<double>(sound.getSample(left)) * (1.0 - frac) +
            static_cast<double>(sound.getSample(left + 1)) * frac;
        stretched.push_back(clampToSample(sample));
    }

    sound.replaceSamples(std::move(stretched));
}
