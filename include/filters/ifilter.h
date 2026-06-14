#ifndef FILTERS_IFILTER_H
#define FILTERS_IFILTER_H

#include "waveform.h"

class IFilter
{
public:
    virtual ~IFilter() = default;
    virtual void apply(Waveform& sound) = 0;
};

#endif
