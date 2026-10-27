
# File Input and Binary Reader — Test Cases

## Module
File Input & Binary Reader

## Requirements
- FCT-F-001: Accept a valid input file path.
- FCT-F-002: Open and read the selected file in binary mode without modifying the original file.

## Test Cases

| Test ID | Test Scenario | Expected Result | Actual Result |
|---|---|---|---|
| TC-01 | Read a text file | File opens and its bytes are read | Passed — 35 bytes |
| TC-02 | Read a binary file | All byte values are read correctly | Passed — 7 bytes |
| TC-03 | Open a missing file | Error is reported and function returns false | Passed |
| TC-04 | Read an empty file | File opens successfully with 0 bytes | Passed |
| TC-05 | Read consecutive files | Previous contents are cleared before the next read | Passed — 35 bytes, then 0 bytes |
| TC-06 | Failed open after a successful read | No stale data remains in the vector | Passed — 35 bytes, then 0 bytes |

## Compilation Command

g++ test_reader.cpp file_reader.cpp -o test_reader

## Execution Command

.\test_reader.exe
