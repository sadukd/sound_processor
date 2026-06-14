#ifndef PARSER_H
#define PARSER_H

#include <optional>
#include <string_view>
#include <vector>

struct FilterDescriptor
{
    std::string_view filterName;
    std::vector<std::string_view> params;
};

enum class Result
{
    ok,
    noArgs,   // show help
    badArgs,  // incorrect arguments sequence
};

class ArgsParser
{
public:
    Result parse(int argc, char* argv[]);

public:
    const std::optional<std::string_view>& getInFileName() const
    {
        return _inFileName;
    }
    const std::optional<std::string_view>& getOutFileName() const
    {
        return _outFileName;
    }
    const std::vector<FilterDescriptor>& getFilters() const { return _filterDescriptors; }

private:
    std::optional<std::string_view> _inFileName;
    std::optional<std::string_view> _outFileName;
    std::vector<FilterDescriptor> _filterDescriptors;
};

#endif
