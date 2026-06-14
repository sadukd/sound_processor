#include "filters/lowpass.h"

#include "filters/filter_utils.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

Lowpass::Lowpass(int windowSize): _windowSize(windowSize)
{
    if(windowSize < 1 || windowSize % 2 == 0)
        throw std::invalid_argument("Lowpass window size must be odd and >= 1");
}

void Lowpass::apply(Waveform& sound)
{
    if(sound.samplesCount() == 0)
        return;

    int radius = _windowSize / 2;
    std::vector<int16_t> filtered;
    filtered.reserve(sound.samplesCount());

    for(size_t i = 0; i < sound.samplesCount(); ++i)
    {
        long long sum = 0;

        for(int offset = -radius; offset <= radius; ++offset)
        {
            long long rawIndex = static_cast<long long>(i) + offset;
            size_t clampedIndex = static_cast<size_t>(
                std::clamp<long long>(rawIndex, 0, sound.samplesCount() - 1));
            sum += sound.getSample(clampedIndex);
        }

        filtered.push_back(
            clampToSample(static_cast<double>(sum) / _windowSize));
    }

    sound.replaceSamples(std::move(filtered));
}
