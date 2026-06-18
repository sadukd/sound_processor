#include "wav_writer.h"
#include "wav_structures.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

void WavWriter::write(const std::string& path, const Waveform& waveform)
{
    if(std::filesystem::path(path).extension() != ".wav")
        throw std::runtime_error("Output file must have .wav extension");

    std::ofstream file(path, std::ios::binary);

    if(!file)
        throw std::runtime_error("Cannot open file");

    uint32_t fileSize = 36 + waveform.samplesCount() * waveform.SAMPLE_SIZE / 8;

    RiffHeader riff{{'R', 'I', 'F', 'F'}, fileSize, {'W', 'A', 'V', 'E'}};

    ChunkHeader fmtHeader{{'f', 'm', 't', ' '}, sizeof(FmtPayload)};

    FmtPayload fmtPayload{waveform.AUDIO_FORMAT,
                          waveform.CHANNEL_COUNT,

                          waveform.SAMPLE_RATE,
                          waveform.SAMPLE_RATE * waveform.CHANNEL_COUNT *
                              waveform.SAMPLE_SIZE / 8,

                          static_cast<uint16_t>(waveform.CHANNEL_COUNT *
                                                waveform.SAMPLE_SIZE / 8),
                          waveform.SAMPLE_SIZE};

    ChunkHeader dataHeader{{'d', 'a', 't', 'a'},
                           static_cast<uint32_t>(waveform.samplesCount() *
                                                 waveform.SAMPLE_SIZE / 8)};

    file.write(reinterpret_cast<const char*>(&riff), sizeof(riff));
    file.write(reinterpret_cast<const char*>(&fmtHeader), sizeof(fmtHeader));
    file.write(reinterpret_cast<const char*>(&fmtPayload), sizeof(fmtPayload));
    file.write(reinterpret_cast<const char*>(&dataHeader), sizeof(dataHeader));

    if(!waveform.getSamples().empty())
    {
        file.write(reinterpret_cast<const char*>(waveform.getSamples().data()),
                   static_cast<std::streamsize>(waveform.samplesCount() *
                                                waveform.SAMPLE_SIZE / 8));
    }
}
