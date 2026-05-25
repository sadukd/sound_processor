#include "parser.h"
#include <set>
#include <string>
#include <cstdint>

enum class Option {
    none,
    input,
    output,
    filter
};

Result ArgsParser::parse(int argc, char* argv[])
{
    if (argc == 1)
    {
        return Result::badArgs;
    }

    Option curOption = Option::none;
    FilterDescriptor* currentFilter = nullptr;

    for (int i = 1; i < argc; i++)
    {
        std::string cur = argv[i];

        if (cur == "-i")
        {
            curOption = Option::input;
            continue;
        }

        if (cur == "-o")
        {
            curOption = Option::output;
            continue;
        }

        if (cur == "-f")
        {
            curOption = Option::filter;
            _filterDescriptors.push_back({});
            currentFilter = &_filterDescriptors.back();
            continue;
        }

        switch (curOption)
        {
        case Option::input:
            if (!_inFileName.empty())
                return Result::badArgs;

            _inFileName = cur;
            curOption = Option::none;
            break;
        case Option::output:
            if (!_outFileName.empty())
                return Result::badArgs;

            _outFileName = cur;
            curOption = Option::none;
            break;
        case Option::filter:
            if (!currentFilter)
                return Result::badArgs;

            if (currentFilter->filterName.empty())
                currentFilter->filterName = cur;
            else
                currentFilter->params.push_back(cur);

            break;

        case Option::none:
            return Result::badArgs;
        }
    }

    return Result::ok;
}
