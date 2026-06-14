#ifndef FILTERS_PRODUCERS_H
#define FILTERS_PRODUCERS_H

#include "pipeline_converter.h"

namespace FilterProducers {
std::unique_ptr<IFilter> createAmplFilter(const FilterDescriptor& descriptor);

std::unique_ptr<IFilter>
createNormalizeFilter(const FilterDescriptor& descriptor);

std::unique_ptr<IFilter>
createSilenceFilter(const FilterDescriptor& descriptor);

std::unique_ptr<IFilter>
createTimeStretchFilter(const FilterDescriptor& descriptor);

std::unique_ptr<IFilter>
createLowpassFilter(const FilterDescriptor& descriptor);

std::unique_ptr<IFilter>
createGeneratorFilter(const FilterDescriptor& descriptor);
}  // namespace FilterProducers

#endif
