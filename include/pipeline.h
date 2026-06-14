#ifndef PIPELINE_H
#define PIPELINE_H

#include "filters/ifilter.h"

#include <cstddef>
#include <memory>
#include <vector>

class Pipeline
{
public:
    void addFilter(std::unique_ptr<IFilter> filter);
    void apply(Waveform& sound) const;

    size_t getFilterCount() const;

private:
    std::vector<std::unique_ptr<IFilter>> _filters;
};

#endif
