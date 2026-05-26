#include "wav_reader.h"
#include "wav_structures.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>

bool char4Cmp(const char a[4], const char* b)
{
    return std::equal(a, a + 4, b);
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

    if(!char4Cmp(riff.chunkId, "RIFF"))
        throw std::runtime_error("Not RIFF file");

    if(!char4Cmp(riff.format, "WAVE"))
        throw std::runtime_error("Not WAVE format");

    FmtPayload fmt{};
    bool fmtFound = false;

    std::vector<int16_t> samples;

    ChunkHeader chunk{};
    while(file.read(reinterpret_cast<char*>(&chunk), sizeof(chunk)))
    {
        if(char4Cmp(chunk.chunkId, "fmt "))
        {
            file.read(reinterpret_cast<char*>(&fmt), sizeof(fmt));

            fmtFound = true;

            if(fmt.bitsPerSample != 16)
                throw std::runtime_error("Only 16-bit PCM supported");

            if(fmt.numChannels != 1)
                throw std::runtime_error("Only mono supported");

            if(chunk.chunkSize > sizeof(fmt))
                file.seekg(chunk.chunkSize - sizeof(fmt), std::ios::cur);

            if(chunk.chunkSize % 2)
                file.seekg(1, std::ios::cur);

            continue;
        }

        if(char4Cmp(chunk.chunkId, "data"))
        {
            if(!fmtFound)
                throw std::runtime_error("Data before fmt");

            samples.resize(chunk.chunkSize / sizeof(uint16_t));

            file.read(reinterpret_cast<char*>(samples.data()), chunk.chunkSize);

            if(chunk.chunkSize % 2)
                file.seekg(1, std::ios::cur);

            break;
        }

        file.seekg(chunk.chunkSize + (chunk.chunkSize % 2), std::ios::cur);
    }

    Waveform wf(samples);

    return wf;
}