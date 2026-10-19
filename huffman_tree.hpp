#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct HuffmanNode
{
    uint8_t symbol;
    std::uint64_t frequency;

    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(uint8_t value, std::uint64_t count)
        : symbol(value),
          frequency(count),
          left(nullptr),
          right(nullptr)
    {
    }

    bool isLeaf() const
    {
        return left == nullptr && right == nullptr;
    }
};

using HuffmanCodeMap = std::map<uint8_t, std::string>;

std::shared_ptr<HuffmanNode> buildHuffmanTree(
    const std::map<uint8_t, std::uint64_t>& frequencies);

HuffmanCodeMap generateHuffmanCodes(
    const std::shared_ptr<HuffmanNode>& root);

#endif