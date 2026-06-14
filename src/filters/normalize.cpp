#include "filters/normalize.h"

#include "filters/filter_utils.h"

#include <cmath>
#include <stdexcept>

Normalize::Normalize(double peak): _peak(peak)
{
    if(peak < 0.0 || peak > 1.0)
        throw std::invalid_argument("Normalize peak must be in range [0, 1]");
}

void Normalize::apply(Waveform& sound)
{
    int currentPeak = 0;

    for(size_t i = 0; i < sound.samplesCount(); ++i)
        currentPeak = std::max(currentPeak,
                               std::abs(static_cast<int>(sound.getSample(i))));

    if(sound.samplesCount() == 0 || currentPeak == 0)
        return;

    double scale = _peak * 32767.0 / static_cast<double>(currentPeak);

    for(size_t i = 0; i < sound.samplesCount(); ++i)
        sound.setSample(
            i, clampToSample(static_cast<double>(sound.getSample(i)) * scale));
}
