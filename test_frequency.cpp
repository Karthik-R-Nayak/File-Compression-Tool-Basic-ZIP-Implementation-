#include "frequency.hpp"
#include "huffman_tree.hpp"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>

using namespace std;

static void writeFile(const string& name, const string& content)
{
    ofstream out(name.c_str(), ios::binary);
    out.write(content.data(), content.size());
}

// FREQ-01: known text
static void testKnownText()
{
    const string text = "aaabbc";
    vector<uint8_t> data(text.begin(), text.end());

    FrequencyTable f = calculateFrequency(data);

    assert(f.size() == 3);
    assert(f['a'] == 3 && f['b'] == 2 && f['c'] == 1);
    cout << "FREQ-01 passed\n";
}

// FREQ-02: empty input
static void testEmpty()
{
    vector<uint8_t> data;
    assert(calculateFrequency(data).empty());
    cout << "FREQ-02 passed\n";
}

// FREQ-03: single repeated symbol
static void testSingleSymbol()
{
    vector<uint8_t> data(1000, 'x');
    FrequencyTable f = calculateFrequency(data);
    assert(f.size() == 1 && f['x'] == 1000);
    cout << "FREQ-03 passed\n";
}

// FREQ-04: binary values incl. 0x00 and bytes above 0x7F
static void testBinaryBytes()
{
    vector<uint8_t> data;
    data.push_back(0x00);
    data.push_back(0xFF);
    data.push_back(0xFF);
    data.push_back(0x80);

    FrequencyTable f = calculateFrequency(data);
    assert(f.size() == 3);
    assert(f[0x00] == 1 && f[0xFF] == 2 && f[0x80] == 1);
    cout << "FREQ-04 passed\n";
}

// FREQ-05: all 256 byte values appear once
static void testAll256()
{
    vector<uint8_t> data;
    for (int i = 0; i < 256; ++i)
        data.push_back(static_cast<uint8_t>(i));

    FrequencyTable f = calculateFrequency(data);
    assert(f.size() == 256);
    for (int i = 0; i < 256; ++i)
        assert(f[static_cast<uint8_t>(i)] == 1);
    cout << "FREQ-05 passed\n";
}

// FREQ-06: reading from a real file path
static void testFromFile()
{
    writeFile("freq_tmp.bin", string("abab\0\xFF", 6));

    FrequencyTable f;
    assert(calculateFrequency("freq_tmp.bin", f));
    assert(f['a'] == 2 && f['b'] == 2 && f[0x00] == 1 && f[0xFF] == 1);

    remove("freq_tmp.bin");
    cout << "FREQ-06 passed\n";
}

// FREQ-07: missing file -> false, table left empty
static void testMissingFile()
{
    FrequencyTable f;
    f['z'] = 5; // stale content must be cleared
    assert(!calculateFrequency("definitely_missing.txt", f));
    assert(f.empty());
    cout << "FREQ-07 passed\n";
}

// FREQ-08: output works directly with the Huffman tree module
static void testWorksWithHuffmanTree()
{
    const string text = "aaabbc";
    vector<uint8_t> data(text.begin(), text.end());

    FrequencyTable f = calculateFrequency(data);
    auto root = buildHuffmanTree(f);
    assert(root != nullptr);

    HuffmanCodeMap codes = generateHuffmanCodes(root);
    assert(codes.size() == 3);
    assert(codes['a'].size() <= codes['c'].size()); // most frequent gets shortest code
    cout << "FREQ-08 passed\n";
}

int main()
{
    testKnownText();
    testEmpty();
    testSingleSymbol();
    testBinaryBytes();
    testAll256();
    testFromFile();
    testMissingFile();
    testWorksWithHuffmanTree();

    cout << "\nAll frequency analysis tests passed.\n";
    return 0;
}
