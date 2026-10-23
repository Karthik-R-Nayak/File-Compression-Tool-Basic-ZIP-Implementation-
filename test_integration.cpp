
#include "file_reader.hpp"
#include "frequency.hpp"
#include "huffman_tree.hpp"
#include "encoder.hpp"

#include <iostream>
#include <unordered_map>

int main()
{
    // Step 1: Read the input file
    std::vector<uint8_t> data;

    if (!readInputFile("sample.txt", data))
        return 1;

    // Step 2: Calculate byte frequencies
    FrequencyTable frequencies = calculateFrequency(data);

    // Step 3: Build the Huffman tree
    auto tree = buildHuffmanTree(frequencies);

    // Step 4: Generate Huffman codes
    HuffmanCodeMap generatedCodes = generateHuffmanCodes(tree);

    // Convert std::map codes to the type required by the Encoder
    std::unordered_map<uint8_t, std::string> codes(
        generatedCodes.begin(), generatedCodes.end());

    // Step 5: Write the compressed file
    if (!writeHuffFile(data, codes, "integration_test.huff", true))
        return 1;

    std::cout << "Integration test passed!" << std::endl;
    std::cout << "Input bytes: " << data.size() << std::endl;
    std::cout << "Distinct symbols: " << frequencies.size() << std::endl;
    std::cout << "Generated codes: " << codes.size() << std::endl;
    std::cout << "Output: integration_test.huff" << std::endl;

    return 0;
}
