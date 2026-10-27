
#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <vector>
#include <string>
#include <cstdint>

bool readInputFile(const std::string& filePath,
                   std::vector<uint8_t>& data);

#endif
