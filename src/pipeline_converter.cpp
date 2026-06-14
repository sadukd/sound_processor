#include "pipeline_converter.h"

#include <stdexcept>

void PipelineConverter::addFilterProducer(const std::string& filterName,
                                          FilterProducer producer)
{
    if(filterName.empty())
        throw std::invalid_argument("Filter producer name cannot be empty");

    if(!producer)
        throw std::invalid_argument("Filter producer cannot be empty");

    _producers[filterName] = std::move(producer);
}

Pipeline PipelineConverter::createPipeline(
    const std::vector<FilterDescriptor>& filters) const
{
    Pipeline pipeline;

    for(const auto& filterDescriptor: filters)
    {
        const std::string filterName(filterDescriptor.filterName);
        auto producerIt = _producers.find(filterName);
        if(producerIt == _producers.end())
            throw std::runtime_error("Unknown filter: " + filterName);

        auto filter = producerIt->second(filterDescriptor);
        if(!filter)
            throw std::runtime_error("Failed to create filter: " + filterName);

        pipeline.addFilter(std::move(filter));
    }

    return pipeline;
}
