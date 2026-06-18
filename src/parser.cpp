#include "parser.h"

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
    _inFileName.reset();
    _outFileName.reset();
    _filterDescriptors.clear();

    if(argc <= 1)
        return Result::noArgs;

    Mode mode = Mode::none;
    FilterDescriptor* currentFilter = nullptr;

    for(int i = 1; i < argc; i++)
    {
        std::string_view token = argv[i];

        if(token == "-i")
        {
            if(_inFileName.has_value() || mode == Mode::input ||
               mode == Mode::output || mode == Mode::filterName)
                return Result::badArgs;

            mode = Mode::input;
            continue;
        }

        if(token == "-o")
        {
            if(_outFileName.has_value() || mode == Mode::input ||
               mode == Mode::output || mode == Mode::filterName)
                return Result::badArgs;

            mode = Mode::output;
            continue;
        }

        if(token == "-f")
        {
            if(mode == Mode::input || mode == Mode::output ||
               mode == Mode::filterName)
                return Result::badArgs;

            _filterDescriptors.push_back(FilterDescriptor{});
            currentFilter = &_filterDescriptors.back();

            mode = Mode::filterName;
            continue;
        }

        if(!token.empty() && token[0] == '-' && mode != Mode::filterArgs)
            return Result::badArgs;

        switch(mode)
        {
        case Mode::input:
            _inFileName = std::move(token);
            mode = Mode::none;
            break;

        case Mode::output:
            _outFileName = std::move(token);
            mode = Mode::none;
            break;

        case Mode::filterName:
            if(!currentFilter)
                return Result::badArgs;

            currentFilter->filterName = token;
            mode = Mode::filterArgs;
            break;

        case Mode::filterArgs:
            if(!currentFilter)
                return Result::badArgs;

            currentFilter->params.push_back(token);
            break;

        case Mode::none:
            return Result::badArgs;
        }
    }

    if(mode == Mode::input || mode == Mode::output || mode == Mode::filterName)
        return Result::badArgs;

    return Result::ok;
}
