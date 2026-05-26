#include "wav_writer.h"

void writer(const std::string& path, const Waveform& wf)
{
    size_t fileSize = 36 + wf.samplesCount() * 
}