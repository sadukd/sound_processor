#include "wav_writer.h"
#include "wav_structures.h"
#include <fstream>
#include <stdexcept>

void WavWriter::writer(const std::string& path, const Waveform& wf)
{
    std::ofstream file(path, std::ios::binary);

    if (!file)
        throw std::runtime_error("Cannot open file");

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
        static_cast<uint32_t>(wf.samplesCount() * wf.SampleSize / 8)
    };

    file.write(
        reinterpret_cast<const char*>(&riff),
        sizeof(riff)
    );

    file.write(
        reinterpret_cast<const char*>(&fmtHeader),
        sizeof(fmtHeader)
    );

    file.write(
        reinterpret_cast<const char*>(&fmtPayload),
        sizeof(fmtPayload)
    );

    file.write(
        reinterpret_cast<const char*>(&dataHeader),
        sizeof(dataHeader)
    );

    file.write(
        reinterpret_cast<const char*>(
            wf.getSamples().data()
        ),
        wf.samplesCount() * wf.SampleSize / 8
    );
}