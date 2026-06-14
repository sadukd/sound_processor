#include <catch2/catch_test_macros.hpp>

#include "filters/producers.h"
#include "pipeline_converter.h"

TEST_CASE("converter builds ordered pipeline")
{
    PipelineConverter converter;
    converter.addFilterProducer("ampl", FilterProducers::createAmplFilter);
    converter.addFilterProducer("normalize",
                                FilterProducers::createNormalizeFilter);

    std::vector<FilterDescriptor> descriptors{
        {"ampl", {"0.5"}},
        {"normalize", {}},
    };

    Pipeline pipeline = converter.createPipeline(descriptors);

    REQUIRE(pipeline.getFilterCount() == 2);
}

TEST_CASE("converter rejects unknown filter")
{
    PipelineConverter converter;

    REQUIRE_THROWS(converter.createPipeline({{"missing", {}}}));
}

TEST_CASE("converter rejects invalid parameters")
{
    PipelineConverter converter;
    converter.addFilterProducer("lowpass",
                                FilterProducers::createLowpassFilter);

    REQUIRE_THROWS(converter.createPipeline({{"lowpass", {"2"}}}));
}

TEST_CASE("generator producer supports subtypes")
{
    PipelineConverter converter;
    converter.addFilterProducer("generator",
                                FilterProducers::createGeneratorFilter);

    Pipeline pipeline =
        converter.createPipeline({{"generator", {"sin", "440", "100"}}});

    REQUIRE(pipeline.getFilterCount() == 1);
}
