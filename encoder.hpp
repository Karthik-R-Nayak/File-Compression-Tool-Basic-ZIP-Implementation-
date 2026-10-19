#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

bool writeHuffFile(
    const std::vector<std::uint8_t>& input,
    const std::unordered_map<std::uint8_t, std::string>& codes,
    const std::string& outputPath,
    bool overwriteExisting = false);

#endif
