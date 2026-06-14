#ifndef FILTERS_GENERATORS_H
#define FILTERS_GENERATORS_H

#include "ifilter.h"

class SineGenerator: public IFilter
{
public:
    SineGenerator(double frequencyHz, double durationMs);

    void apply(Waveform& sound) override;

private:
    double _frequencyHz;
    double _durationMs;
};

class AmGenerator: public IFilter
{
public:
    AmGenerator(double amplitude, double carrierHz, double modulationHz,
                double depth, double durationMs);

    void apply(Waveform& sound) override;

private:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _depth;
    double _durationMs;
};

class FmGenerator: public IFilter
{
public:
    FmGenerator(double amplitude, double carrierHz, double modulationHz,
                double deviationHz, double durationMs);

    void apply(Waveform& sound) override;

private:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _deviationHz;
    double _durationMs;
};

#endif
