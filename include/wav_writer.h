#ifndef WAV_WRITER_H
#define WAV_WRITER_H

#include "waveform.h"
#include <string>

class WavWriter
{
public:
    void write(const std::string& path, const Waveform& waveform);
};

#endif
