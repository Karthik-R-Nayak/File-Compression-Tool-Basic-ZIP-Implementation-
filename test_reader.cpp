
#include <iostream>
#include "file_reader.hpp"

using namespace std;

int main()
{
    vector<uint8_t> data;

    if (!readInputFile("sample.txt", data))
        return 1;

    cout << "First file bytes: " << data.size() << endl;

    if (!readInputFile("empty.txt", data))
        return 1;

    cout << "Second file bytes: " << data.size() << endl;

    return 0;
}
