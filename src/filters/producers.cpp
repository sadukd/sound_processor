#include "filters/producers.h"

#include "filters/ampl.h"
#include "filters/generators.h"
#include "filters/lowpass.h"
#include "filters/normalize.h"
#include "filters/silence.h"
#include "filters/timestretch.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

namespace FilterProducers {
namespace {
double parseDouble(std::string_view raw, const std::string& filterName)
{
    try
    {
        const std::string RAW_STRING(raw);
        size_t pos = 0;
        double value = std::stod(RAW_STRING, &pos);
        if(pos != RAW_STRING.size())
            throw std::invalid_argument("Trailing characters");
        return value;
    }
    catch(const std::exception&)
    {
        throw std::runtime_error("Invalid numeric parameter for filter: " +
                                 filterName);
    }
}

int parseInt(std::string_view raw, const std::string& filterName)
{
    try
    {
        const std::string RAW_STRING(raw);
        size_t pos = 0;
        int value = std::stoi(RAW_STRING, &pos);
        if(pos != RAW_STRING.size())
            throw std::invalid_argument("Trailing characters");
        return value;
    }
    catch(const std::exception&)
    {
        throw std::runtime_error("Invalid integer parameter for filter: " +
                                 filterName);
    }
}
}  // namespace

std::unique_ptr<IFilter> createAmplFilter(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 1)
        throw std::runtime_error("Filter ampl expects 1 parameter");

    return std::make_unique<Ampl>(parseDouble(descriptor.params[0], "ampl"));
}

std::unique_ptr<IFilter>
createNormalizeFilter(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() > 1)
        throw std::runtime_error("Filter normalize expects 0 or 1 parameters");

    double peak = descriptor.params.empty()
                      ? 1.0
                      : parseDouble(descriptor.params[0], "normalize");
    return std::make_unique<Normalize>(peak);
}

std::unique_ptr<IFilter> createSilenceFilter(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 3)
        throw std::runtime_error("Filter silence expects 3 parameters");

    return std::make_unique<Silence>(
        std::string(descriptor.params[0]),
        parseDouble(descriptor.params[1], "silence"),
        parseDouble(descriptor.params[2], "silence"));
}

std::unique_ptr<IFilter>
createTimeStretchFilter(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 1)
        throw std::runtime_error("Filter timestretch expects 1 parameter");

    return std::make_unique<TimeStretch>(
        parseDouble(descriptor.params[0], "timestretch"));
}

std::unique_ptr<IFilter> createLowpassFilter(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 1)
        throw std::runtime_error("Filter lowpass expects 1 parameter");

    return std::make_unique<Lowpass>(parseInt(descriptor.params[0], "lowpass"));
}

std::unique_ptr<IFilter>
createGeneratorFilter(const FilterDescriptor& descriptor)
{
    if(descriptor.params.empty())
        throw std::runtime_error("Filter generator expects a generator type");

    const std::string_view GENERATOR_TYPE = descriptor.params[0];

    if(GENERATOR_TYPE == "sin")
    {
        if(descriptor.params.size() != 3)
            throw std::runtime_error("Generator sin expects 2 parameters");

        return std::make_unique<SineGenerator>(
            parseDouble(descriptor.params[1], "generator sin"),
            parseDouble(descriptor.params[2], "generator sin"));
    }

    if(GENERATOR_TYPE == "am")
    {
        if(descriptor.params.size() != 6)
            throw std::runtime_error("Generator am expects 5 parameters");

        return std::make_unique<AmGenerator>(
            parseDouble(descriptor.params[1], "generator am"),
            parseDouble(descriptor.params[2], "generator am"),
            parseDouble(descriptor.params[3], "generator am"),
            parseDouble(descriptor.params[4], "generator am"),
            parseDouble(descriptor.params[5], "generator am"));
    }

    if(GENERATOR_TYPE == "fm")
    {
        if(descriptor.params.size() != 6)
            throw std::runtime_error("Generator fm expects 5 parameters");

        return std::make_unique<FmGenerator>(
            parseDouble(descriptor.params[1], "generator fm"),
            parseDouble(descriptor.params[2], "generator fm"),
            parseDouble(descriptor.params[3], "generator fm"),
            parseDouble(descriptor.params[4], "generator fm"),
            parseDouble(descriptor.params[5], "generator fm"));
    }

    throw std::runtime_error("Unknown generator type: " +
                             std::string(GENERATOR_TYPE));
}
}  // namespace FilterProducers
