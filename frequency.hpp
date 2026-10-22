#ifndef FREQUENCY_HPP
#define FREQUENCY_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>

// Frequency table: byte value -> number of occurrences.
// Only bytes that actually occur are stored (no zero entries), which is
// exactly what buildHuffmanTree() in huffman_tree.hpp expects.
using FrequencyTable = std::map<std::uint8_t, std::uint64_t>;

// Counts how many times each byte occurs in already-loaded data
// (the vector produced by readInputFile()). Empty input -> empty table.
FrequencyTable calculateFrequency(const std::vector<std::uint8_t>& data);

// Convenience overload (matches SAD 4.3): reads the file in binary mode via
// readInputFile() and counts its bytes.
// Returns false (and leaves 'frequencies' empty) if the file cannot be read.
bool calculateFrequency(const std::string& filePath,
                        FrequencyTable& frequencies);

#endif
