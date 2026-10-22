# Frequency Analysis Module — Test Cases

## Module
Frequency Analysis (`frequency.hpp` / `frequency.cpp`)

## Requirements
- FCT-F-003: Calculate the frequency of each byte/symbol present in the input file.
- Test plan reference: TC-HUFF-01

## Design
`calculateFrequency` returns a `FrequencyTable` (`std::map<uint8_t, uint64_t>`) that contains
only bytes which actually occur. This is the exact input type of `buildHuffmanTree()`.

| Function | Purpose |
|---|---|
| `FrequencyTable calculateFrequency(const std::vector<uint8_t>&)` | Count bytes already loaded by `readInputFile()` |
| `bool calculateFrequency(const std::string&, FrequencyTable&)` | Read a file (binary) and count it; `false` if unreadable |

## Test Cases

| Test ID | Scenario | Expected Result |
|---|---|---|
| FREQ-01 | Known text `aaabbc` | a=3, b=2, c=1 |
| FREQ-02 | Empty input | Empty table |
| FREQ-03 | 1000 copies of one byte | One entry with count 1000 |
| FREQ-04 | Bytes `0x00`, `0xFF`, `0xFF`, `0x80` | Counts 1, 2, 1 (no sign/overflow errors) |
| FREQ-05 | All 256 byte values once | 256 entries, each count 1 |
| FREQ-06 | Count from a real binary file path | Correct counts |
| FREQ-07 | Missing file | Returns `false`; table cleared |
| FREQ-08 | Feed result into `buildHuffmanTree` / `generateHuffmanCodes` | Valid tree; most frequent symbol gets the shortest code |

## Compilation and Execution

Windows:

```text
g++ -std=c++11 test_frequency.cpp frequency.cpp file_reader.cpp huffman_tree.cpp -o test_frequency
.\test_frequency.exe
```

Linux:

```text
g++ -std=c++11 test_frequency.cpp frequency.cpp file_reader.cpp huffman_tree.cpp -o test_frequency
./test_frequency
```
