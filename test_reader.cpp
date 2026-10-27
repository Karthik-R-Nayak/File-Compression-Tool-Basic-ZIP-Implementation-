
#include <iostream>
#include "file_reader.hpp"

using namespace std;

int main()
{
    vector<uint8_t> data;

    if (!readInputFile("sample.txt", data))
        return 1;

    cout << "Before failed read: " << data.size() << endl;

    if (readInputFile("missing_file.txt", data))
    {
        cout << "Unexpected success!" << endl;
        return 1;
    }

    cout << "After failed open: " << data.size() << endl;

    return 0;
}
