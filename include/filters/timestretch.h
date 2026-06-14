#ifndef FILTERS_TIMESTRETCH_H
#define FILTERS_TIMESTRETCH_H

#include "ifilter.h"

class TimeStretch: public IFilter
{
public:
    explicit TimeStretch(double factor);

    void apply(Waveform& sound) override;

private:
    double _factor;
};

#endif
