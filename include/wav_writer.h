#pragma once

#include <string>
#include "waveform.h"

class WavWriter
{
public:
    void writer(const std::string& path, const Waveform& wf);
};