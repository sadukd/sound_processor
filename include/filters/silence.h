#ifndef FILTERS_SILENCE_H
#define FILTERS_SILENCE_H

#include "ifilter.h"

#include <string>

class Silence: public IFilter
{
public:
    Silence(const std::string& unit, double start, double end);

    void apply(Waveform& sound) override;

private:
    size_t timeToSamples(double value) const;

private:
    std::string _unit;
    double _start;
    double _end;
};

#endif
