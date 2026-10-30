#include "cli.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; ++i)
        arguments.push_back(argv[i]);

    return runCli(arguments, std::cout, std::cerr);
}
