#include "frequency.hpp"
#include "file_reader.hpp"

FrequencyTable calculateFrequency(const std::vector<std::uint8_t>& data)
{
    FrequencyTable frequencies;

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        ++frequencies[data[i]];
    }

    return frequencies;
}

bool calculateFrequency(const std::string& filePath,
                        FrequencyTable& frequencies)
{
    frequencies.clear();

    std::vector<std::uint8_t> data;
    if (!readInputFile(filePath, data))
        return false; // readInputFile() already reported the error

    frequencies = calculateFrequency(data);
    return true;
}
