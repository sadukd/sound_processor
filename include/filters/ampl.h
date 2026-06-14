#ifndef FILTERS_AMPL_H
#define FILTERS_AMPL_H

#include "ifilter.h"

class Ampl: public IFilter
{
public:
    explicit Ampl(double factor);

    void apply(Waveform& sound) override;

private:
    double _factor;
};

#endif
