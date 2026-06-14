#include "application.h"

#include "filters/producers.h"
#include "parser.h"
#include "wav_reader.h"
#include "wav_writer.h"

#include <iostream>

void Application::configure()
{
    _pipelineConverter.addFilterProducer("ampl",
                                         FilterProducers::createAmplFilter);
    _pipelineConverter.addFilterProducer(
        "normalize", FilterProducers::createNormalizeFilter);
    _pipelineConverter.addFilterProducer("silence",
                                         FilterProducers::createSilenceFilter);
    _pipelineConverter.addFilterProducer(
        "timestretch", FilterProducers::createTimeStretchFilter);
    _pipelineConverter.addFilterProducer("lowpass",
                                         FilterProducers::createLowpassFilter);
    _pipelineConverter.addFilterProducer(
        "generator", FilterProducers::createGeneratorFilter);
}

int Application::start(int argc, char* argv[])
{
    ArgsParser parser;
    Result result = parser.parse(argc, argv);

    if(result == Result::noArgs)
    {
        printHelp();
        return 0;
    }

    if(result != Result::ok)
    {
        printHelp();
        return 1;
    }

    Waveform sound;

    if(parser.getInFileName().has_value())
    {
        WavReader reader;
        sound = reader.read(std::string(*parser.getInFileName()));
    }

    Pipeline pipeline = _pipelineConverter.createPipeline(parser.getFilters());
    pipeline.apply(sound);

    if(parser.getOutFileName().has_value())
    {
        WavWriter writer;
        writer.write(std::string(*parser.getOutFileName()), sound);
    }

    return 0;
}

void Application::printHelp() const
{
    std::cout << "Usage: sound_processor [-i input.wav] [-o output.wav] "
                 "[-f filter_name [params...]]...\n"
              << "Filters: ampl, normalize, silence, timestretch, lowpass, "
                 "generator\n";
}
