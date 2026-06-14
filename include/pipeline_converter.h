#ifndef PIPELINE_CONVERTER_H
#define PIPELINE_CONVERTER_H

#include "parser.h"
#include "pipeline.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

using FilterProducer =
    std::function<std::unique_ptr<IFilter>(const FilterDescriptor&)>;

class PipelineConverter
{
public:
    void addFilterProducer(const std::string& filterName,
                           FilterProducer producer);
    Pipeline createPipeline(const std::vector<FilterDescriptor>& filters) const;

private:
    std::unordered_map<std::string, FilterProducer> _producers;
};

#endif
