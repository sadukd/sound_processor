#ifndef WAV_READER_H
#define WAV_READER_H

#include "waveform.h"
#include <string>

class WavReader
{
public:
    Waveform read(const std::string& path);
};

#endif
