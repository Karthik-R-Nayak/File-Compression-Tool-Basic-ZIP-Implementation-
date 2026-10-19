#include <iostream>
#include <vector>
#include <cstdint>
#include "file_reader.cpp"

using namespace std;

int main()
{
    vector<uint8_t> data;

    if (readInputFile("binary_sample.bin", data))
    {
        cout << "File read successfully!" << endl;
        cout << "Bytes read: " << data.size() << endl;
    }

    return 0;
}