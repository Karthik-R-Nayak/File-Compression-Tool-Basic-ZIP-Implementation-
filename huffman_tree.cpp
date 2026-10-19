#include "huffman_tree.hpp"

#include <queue>
#include <stdexcept>
#include <utility>

namespace
{
    struct CompareNodes
    {
        bool operator()(
            const std::shared_ptr<HuffmanNode>& a,
            const std::shared_ptr<HuffmanNode>& b) const
        {
            if (a->frequency != b->frequency)
                return a->frequency > b->frequency;

            return a->symbol > b->symbol;
        }
    };

    void generateCodesRecursive(
        const std::shared_ptr<HuffmanNode>& node,
        const std::string& prefix,
        HuffmanCodeMap& codes)
    {
        if (!node)
            return;

        if (node->isLeaf())
        {
            // A single-symbol file needs a non-empty code.
            codes[node->symbol] = prefix.empty() ? "0" : prefix;
            return;
        }

        generateCodesRecursive(node->left, prefix + "0", codes);
        generateCodesRecursive(node->right, prefix + "1", codes);
    }
}

std::shared_ptr<HuffmanNode> buildHuffmanTree(
    const std::map<uint8_t, std::uint64_t>& frequencies)
{
    if (frequencies.empty())
        return nullptr;

    std::priority_queue<
        std::shared_ptr<HuffmanNode>,
        std::vector<std::shared_ptr<HuffmanNode>>,
        CompareNodes> queue;

    for (const auto& entry : frequencies)
    {
        if (entry.second == 0)
            throw std::invalid_argument("Symbol frequency must be greater than zero.");

        queue.push(std::make_shared<HuffmanNode>(
            entry.first, entry.second));
    }

    while (queue.size() > 1)
    {
        auto left = queue.top();
        queue.pop();

        auto right = queue.top();
        queue.pop();

        if (left->frequency >
            UINT64_MAX - right->frequency)
        {
            throw std::overflow_error("Huffman frequency total overflow.");
        }

        auto parent = std::make_shared<HuffmanNode>(
            left->symbol,
            left->frequency + right->frequency);

        parent->left = std::move(left);
        parent->right = std::move(right);

        queue.push(std::move(parent));
    }

    return queue.top();
}

HuffmanCodeMap generateHuffmanCodes(
    const std::shared_ptr<HuffmanNode>& root)
{
    HuffmanCodeMap codes;
    generateCodesRecursive(root, "", codes);
    return codes;
}