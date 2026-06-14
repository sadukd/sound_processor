#ifndef FILTERS_NORMALIZE_H
#define FILTERS_NORMALIZE_H

#include "ifilter.h"

class Normalize: public IFilter
{
public:
    explicit Normalize(double peak = 1.0);

    void apply(Waveform& sound) override;

private:
    double _peak;
};

#endif
