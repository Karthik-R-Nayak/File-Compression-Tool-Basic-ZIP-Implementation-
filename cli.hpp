#ifndef CLI_HPP
#define CLI_HPP

#include <iosfwd>
#include <string>
#include <vector>

enum class CliCommand
{
    Help,
    Compress
};

struct CliOptions
{
    CliCommand command;
    std::string inputPath;
    std::string outputPath;

    CliOptions()
        : command(CliCommand::Help)
    {
    }
};

bool parseCliArguments(const std::vector<std::string>& arguments,
                       CliOptions& options,
                       std::string& errorMessage);

void printCliUsage(std::ostream& output);

int runCli(const std::vector<std::string>& arguments,
           std::ostream& output,
           std::ostream& errors);

#endif
