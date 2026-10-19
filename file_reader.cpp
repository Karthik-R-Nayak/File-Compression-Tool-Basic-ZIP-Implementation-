
#include "file_reader.hpp"
#include <iostream>
#include <fstream>

using namespace std;

bool readInputFile(const string& filePath, vector<uint8_t>& data)
{
    data.clear();

    ifstream inputFile(filePath, ios::binary);

    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open input file: "
             << filePath << endl;
        return false;
    }

    char byte;

    while (inputFile.get(byte))
    {
        data.push_back(static_cast<uint8_t>(
            static_cast<unsigned char>(byte)));
    }

    if (inputFile.bad())
    {
        cerr << "Error: Failed while reading input file."
             << endl;
        return false;
    }

    return true;
}
