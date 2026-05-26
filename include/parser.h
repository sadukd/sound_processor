#pragma once

#include <vector>
#include <string>

struct FilterDescriptor
{
    std::string filterName;
    std::vector<std::string> params;
};

enum class Result {
    ok,
    noArgs,		// show help
    badArgs,	// incorrect arguments sequence
};

class ArgsParser {
public:
    Result parse(int argc, char* argv[]);
public:
    const std::string& getInFileName() const { return _inFileName; }
    const std::string& getOutFileName() const { return _outFileName; }
    const std::vector<FilterDescriptor>& getFilters() const { return _filterDescriptors; }

private:
    std::string _inFileName;
    std::string _outFileName;
    std::vector<FilterDescriptor> _filterDescriptors;    
};
