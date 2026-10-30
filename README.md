# File Compression Tool

This command-line tool compresses files into the project's version 1 `.huff`
format using Huffman coding.

## Build

From the repository root, build with a C++11-compatible compiler:

```sh
g++ -std=c++11 main.cpp cli.cpp file_reader.cpp frequency.cpp huffman_tree.cpp encoder.cpp -o file-compressor
```

On Windows, the resulting executable can be named `file-compressor.exe`.

## Usage

```text
file-compressor compress <input> [-o|--output <path>]
file-compressor --help
```

If `-o` / `--output` is omitted, the output path is `<input>.huff`. The tool
does not replace an existing output file unless overwrite protection is later
explicitly confirmed or configured; the current encoder rejects such a write.

Decompression is not available yet. It depends on implementation of
FCT-F-010 through FCT-F-014.
