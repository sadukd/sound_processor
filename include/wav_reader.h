#pragma once

#include "waveform.h"
#include <string>

class WavReader
{
public:
    Waveform read(const std::string& path);
};