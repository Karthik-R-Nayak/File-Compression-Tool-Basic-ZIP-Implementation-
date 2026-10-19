#include "encoder.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <limits>

using std::uint8_t;
using std::uint16_t;
using std::uint64_t;

namespace
{
    bool writeByte(std::ostream& output, uint8_t value)
    {
        output.write(reinterpret_cast<const char*>(&value), 1);
        return static_cast<bool>(output);
    }

    bool writeLittleEndian(std::ostream& output,
                           uint64_t value,
                           unsigned int byteCount)
    {
        for (unsigned int i = 0; i < byteCount; ++i)
        {
            if (!writeByte(output, static_cast<uint8_t>(value & 0xff)))
                return false;
            value >>= 8;
        }
        return true;
    }
}

bool writeHuffFile(
    const std::vector<std::uint8_t>& input,
    const std::unordered_map<std::uint8_t, std::string>& codes,
    const std::string& outputPath,
    bool overwriteExisting)
{
    std::array<uint64_t, 256> frequencies = {};
    uint64_t originalSize = 0;

    for (uint8_t symbol : input)
    {
        if (originalSize == std::numeric_limits<uint64_t>::max())
        {
            std::cerr << "Error: Input is too large for the .huff format."
                      << std::endl;
            return false;
        }
        ++originalSize;
        ++frequencies[symbol];
    }

    uint64_t bitCount = 0;
    for (std::size_t symbol = 0; symbol < frequencies.size(); ++symbol)
    {
        if (frequencies[symbol] == 0)
            continue;

        const auto code = codes.find(static_cast<uint8_t>(symbol));
        if (code == codes.end() || code->second.empty())
        {
            std::cerr << "Error: Missing Huffman code for an input symbol."
                      << std::endl;
            return false;
        }

        uint64_t codeLength = 0;
        for (char bit : code->second)
        {
            if (bit != '0' && bit != '1')
            {
                std::cerr << "Error: Huffman codes must contain only 0 and 1."
                          << std::endl;
                return false;
            }
            if (codeLength == std::numeric_limits<uint64_t>::max())
            {
                std::cerr << "Error: Encoded bitstream is too large."
                          << std::endl;
                return false;
            }
            ++codeLength;
        }

        if (codeLength != 0 &&
            frequencies[symbol] >
                (std::numeric_limits<uint64_t>::max() - bitCount) /
                    codeLength)
        {
            std::cerr << "Error: Encoded bitstream is too large."
                      << std::endl;
            return false;
        }
        bitCount += frequencies[symbol] * codeLength;
    }

    if (!overwriteExisting)
    {
        std::ifstream existing(outputPath.c_str(), std::ios::binary);
        if (existing.is_open())
        {
            std::cerr << "Error: Output file already exists: "
                      << outputPath << std::endl;
            return false;
        }
    }

    std::ofstream output(outputPath.c_str(), std::ios::binary | std::ios::trunc);
    if (!output.is_open())
    {
        std::cerr << "Error: Could not open output file: "
                  << outputPath << std::endl;
        return false;
    }

    const char magic[] = {'H', 'U', 'F', 'F'};
    output.write(magic, sizeof(magic));
    if (!output ||
        !writeByte(output, 1) ||
        !writeLittleEndian(output, originalSize, 8) ||
        !writeLittleEndian(output, bitCount, 8))
    {
        std::cerr << "Error: Failed while writing .huff header."
                  << std::endl;
        return false;
    }

    uint16_t symbolCount = 0;
    for (uint64_t frequency : frequencies)
    {
        if (frequency != 0)
            ++symbolCount;
    }

    if (!writeLittleEndian(output, symbolCount, 2))
    {
        std::cerr << "Error: Failed while writing .huff header."
                  << std::endl;
        return false;
    }

    for (std::size_t symbol = 0; symbol < frequencies.size(); ++symbol)
    {
        if (frequencies[symbol] == 0)
            continue;
        if (!writeByte(output, static_cast<uint8_t>(symbol)) ||
            !writeLittleEndian(output, frequencies[symbol], 8))
        {
            std::cerr << "Error: Failed while writing .huff header."
                      << std::endl;
            return false;
        }
    }

    uint8_t packedByte = 0;
    unsigned int bitsInByte = 0;
    for (uint8_t symbol : input)
    {
        const std::string& code = codes.find(symbol)->second;
        for (char bit : code)
        {
            packedByte = static_cast<uint8_t>(
                (packedByte << 1) | static_cast<uint8_t>(bit == '1'));
            ++bitsInByte;

            if (bitsInByte == 8)
            {
                if (!writeByte(output, packedByte))
                {
                    std::cerr << "Error: Failed while writing compressed data."
                              << std::endl;
                    return false;
                }
                packedByte = 0;
                bitsInByte = 0;
            }
        }
    }

    if (bitsInByte != 0)
    {
        packedByte = static_cast<uint8_t>(
            packedByte << (8 - bitsInByte));
        if (!writeByte(output, packedByte))
        {
            std::cerr << "Error: Failed while writing compressed data."
                      << std::endl;
            return false;
        }
    }

    output.close();
    if (!output)
    {
        std::cerr << "Error: Failed while finalizing output file: "
                  << outputPath << std::endl;
        return false;
    }

    return true;
}
