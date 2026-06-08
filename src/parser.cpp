#include "parser.h"
#include <string>
#include <vector>

enum class Mode
{
    none,
    input,
    output,
    filterName,
    filterArgs
};

Result ArgsParser::parse(int argc, char* argv[])
{
    if(argc <= 1)
        return Result::badArgs;

    Mode mode = Mode::none;
    FilterDescriptor* currentFilter = nullptr;

    for(int i = 1; i < argc; i++)
    {
        std::string cur = argv[i];

        if(cur == "-i")
        {
            if(!_inFileName.empty())
                return Result::badArgs;

            mode = Mode::input;
            continue;
        }

        if(cur == "-o")
        {
            if(!_outFileName.empty())
                return Result::badArgs;

            mode = Mode::output;
            continue;
        }

        if(cur == "-f")
        {
            _filterDescriptors.push_back(FilterDescriptor{});
            currentFilter = &_filterDescriptors.back();

            mode = Mode::filterName;
            continue;
        }

        if(cur[0] == '-')
            return Result::badArgs;

        switch(mode)
        {
        case Mode::input:
            _inFileName = cur;
            mode = Mode::none;
            break;

        case Mode::output:
            _outFileName = cur;
            mode = Mode::none;
            break;

        case Mode::filterName:
            if(!currentFilter)
                return Result::badArgs;

            currentFilter->filterName = cur;
            mode = Mode::filterArgs;
            break;

        case Mode::filterArgs:
            if(!currentFilter)
                return Result::badArgs;

            currentFilter->params.push_back(cur);
            break;

        case Mode::none:
            return Result::badArgs;
        }
    }

    if(_outFileName.empty())
        return Result::badArgs;

    return Result::ok;
}