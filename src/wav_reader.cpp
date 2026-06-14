#include "wav_reader.h"
#include "wav_structures.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>

bool hasChunkId(const char left[4], const char* right)
{
    return std::equal(left, left + 4, right);
}

Waveform WavReader::read(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if(!file)
        throw std::runtime_error("Cannot open file");

    RiffHeader riff{};
    file.read(reinterpret_cast<char*>(&riff), sizeof(riff));

    if(!file)
        throw std::runtime_error("Invalid RIFF header");

    if(!hasChunkId(riff.chunkId, "RIFF"))
        throw std::runtime_error("Not RIFF file");

    if(!hasChunkId(riff.format, "WAVE"))
        throw std::runtime_error("Not WAVE format");

    FmtPayload fmt{};
    bool fmtFound = false;

    std::vector<int16_t> samples;

    ChunkHeader chunk{};
    while(file.read(reinterpret_cast<char*>(&chunk), sizeof(chunk)))
    {
        if(hasChunkId(chunk.chunkId, "fmt "))
        {
            if(chunk.chunkSize < sizeof(fmt))
                throw std::runtime_error("Invalid fmt chunk size");

            file.read(reinterpret_cast<char*>(&fmt), sizeof(fmt));
            if(!file)
                throw std::runtime_error("Invalid fmt chunk");

            fmtFound = true;

            if(fmt.compressionCode != Waveform::AUDIO_FORMAT)
                throw std::runtime_error("Only PCM WAV files are supported");

            if(fmt.bitsPerSample != 16)
                throw std::runtime_error("Only 16-bit PCM supported");

            if(fmt.numChannels != 1)
                throw std::runtime_error("Only mono supported");

            if(fmt.sampleRate != Waveform::SAMPLE_RATE)
                throw std::runtime_error(
                    "Only 44100 Hz WAV files are supported");

            if(chunk.chunkSize > sizeof(fmt))
                file.seekg(chunk.chunkSize - sizeof(fmt), std::ios::cur);

            if(chunk.chunkSize % 2)
                file.seekg(1, std::ios::cur);

            continue;
        }

        if(hasChunkId(chunk.chunkId, "data"))
        {
            if(!fmtFound)
                throw std::runtime_error("Data before fmt");

            if(chunk.chunkSize % sizeof(int16_t) != 0)
                throw std::runtime_error(
                    "Data chunk is not aligned to 16-bit samples");

            samples.resize(chunk.chunkSize / sizeof(int16_t));

            file.read(reinterpret_cast<char*>(samples.data()), chunk.chunkSize);
            if(!file)
                throw std::runtime_error("Invalid data chunk");

            if(chunk.chunkSize % 2)
                file.seekg(1, std::ios::cur);

            break;
        }

        file.seekg(chunk.chunkSize + (chunk.chunkSize % 2), std::ios::cur);
    }

    return Waveform(samples);
}
