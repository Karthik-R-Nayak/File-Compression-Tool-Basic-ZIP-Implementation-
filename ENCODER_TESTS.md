# Encoder and Header Writer — Test Cases

## Module
Encoder and `.huff` file writer

## Requirements
- FCT-F-006: Encode input symbols into an MSB-first bitstream.
- FCT-F-007: Store decompression metadata in the file header.
- FCT-F-008: Create the compressed file at the requested destination.
- FCT-NF-005: Use portable standard C++ and a documented byte order.

## File Format, Version 1

All integer fields are unsigned and little-endian. The file consists of:

| Field | Size | Description |
|---|---:|---|
| Magic | 4 bytes | ASCII `HUFF` |
| Version | 1 byte | `1` |
| Original size | 8 bytes | Number of uncompressed input bytes |
| Encoded bit count | 8 bytes | Number of meaningful bits in the payload |
| Symbol count | 2 bytes | Number of symbols in the frequency table (0–256) |
| Frequency table | 9 bytes per symbol | Symbol byte, then its 8-byte frequency |
| Payload | Variable | Huffman code bits, most-significant bit first |

Frequency entries are written in ascending symbol-byte order. Unused low bits
in the final payload byte are zero. The original size and encoded bit count
allow a decoder to distinguish meaningful bits from padding. Empty input has
an empty frequency table and payload.

## Test Cases

| Test ID | Scenario | Expected Result |
|---|---|---|
| ENC-01 | Encode `ABA` with `A=0`, `B=11` | Correct metadata; payload is `0110 0000` |
| ENC-02 | Encode empty input with an empty code table | Valid empty `.huff` header and no payload |
| ENC-03 | Missing code for an input symbol | Return `false` before opening the output |
| ENC-04 | Non-binary or empty code for an occurring symbol | Return `false` and report the invalid code |
| ENC-05 | Output path cannot be opened | Return `false` and report the output error |
| ENC-06 | Destination exists and overwrite is not enabled | Return `false`; existing bytes remain unchanged |
| ENC-07 | Destination exists and overwrite is enabled | Replace it with the generated `.huff` file |

The writer does not overwrite by default. Pass `true` as the final
`writeHuffFile` argument only after the calling application has obtained
overwrite confirmation.

## Compilation and Execution

Windows:

```text
g++ -std=c++11 test_encoder.cpp encoder.cpp -o test_encoder
.\test_encoder.exe
```

Linux:

```text
g++ -std=c++11 test_encoder.cpp encoder.cpp -o test_encoder
./test_encoder
```
