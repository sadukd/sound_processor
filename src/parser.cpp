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

    for (int i = 0; i < argc; i++)
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
            _filterDescriptors.push_back({})ж
            currentFilter = &_filterDescriptors.back()
            continue;
        }

        switch (curOption)
        {
        case State::input:
            if (!_inFileName.empty())
                return Result::badArgs;

            _inFileName = cur;
            curOption = State::None;
            break;
        case State::Output:
            if (!_outFileName.empty())
                return Result::badArgs;

            _outFileName = cur;
            curOption = State::None;
            break;
        case State::Filter:
            if (!currentFilter)
                return Result::badArgs;

            if (currentFilter->name.empty())
                currentFilter->name = cur;
            else
                currentFilter->params.push_back(cur);

            break;

        case State::None:
            return Result::badArgs;
        }
    }

    return Result::ok;
}
