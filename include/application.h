#ifndef APPLICATION_H
#define APPLICATION_H

#include "pipeline_converter.h"

class Application
{
public:
    void configure();
    int start(int argc, char* argv[]);

private:
    void printHelp() const;

private:
    PipelineConverter _pipelineConverter;
};

#endif
