#ifndef FILTERS_LOWPASS_H
#define FILTERS_LOWPASS_H

#include "ifilter.h"

class Lowpass: public IFilter
{
public:
    explicit Lowpass(int windowSize);

    void apply(Waveform& sound) override;

private:
    int _windowSize;
};

#endif
