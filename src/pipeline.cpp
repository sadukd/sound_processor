#include "pipeline.h"

#include <stdexcept>

void Pipeline::addFilter(std::unique_ptr<IFilter> filter)
{
    if(!filter)
        throw std::invalid_argument("Cannot add null filter to pipeline");

    _filters.push_back(std::move(filter));
}

void Pipeline::apply(Waveform& sound) const
{
    for(const auto& filter: _filters)
        filter->apply(sound);
}

size_t Pipeline::getFilterCount() const { return _filters.size(); }
