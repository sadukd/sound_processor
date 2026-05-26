#include "wav_writer.h"
#include "wav_structures.h"

void writer(const std::string& path, const Waveform& wf)
{
    uint32_t fileSize = 36 + wf.samplesCount() * wf.SampleSize / 8;

    RiffHeader riff{
        {'R', 'I', 'F', 'F'},
        fileSize,
        {'W', 'A', 'V', 'E'}
    };

    ChunkHeader fmtHeader{
        {'f', 'm', 't', ' '},
        sizeof(FmtPayload)
    };

    FmtPayload fmtPayload{
        wf.AudioFormat,
        wf.ChannelAmnt,

        wf.SampleRate,
        wf.SampleRate * wf.ChannelAmnt *
        wf.SampleSize / 8,

        static_cast<uint16_t>(
            wf.ChannelAmnt *
            wf.SampleSize / 8
        ),
        wf.SampleSize
    };

    ChunkHeader dataHeader{
        {'d', 'a', 't', 'a'},
        wf.samplesCount() * wf.SampleSize / 8
    };

    
}