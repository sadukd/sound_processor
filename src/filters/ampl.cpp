#include "filters/ampl.h"

#include "filters/filter_utils.h"

#include <stdexcept>

Ampl::Ampl(double factor): _factor(factor)
{
    if(factor < 0.0)
        throw std::invalid_argument("Ampl factor must be non-negative");
}

void Ampl::apply(Waveform& sound)
{
    for(size_t i = 0; i < sound.samplesCount(); ++i)
        sound.setSample(
            i,
            clampToSample(static_cast<double>(sound.getSample(i)) * _factor));
}
