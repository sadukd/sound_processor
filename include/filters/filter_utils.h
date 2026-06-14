#ifndef FILTERS_FILTER_UTILS_H
#define FILTERS_FILTER_UTILS_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

inline int16_t clampToSample(double value)
{
    value = std::clamp(
        value, static_cast<double>(std::numeric_limits<int16_t>::min()),
        static_cast<double>(std::numeric_limits<int16_t>::max()));
    return static_cast<int16_t>(std::round(value));
}

#endif
