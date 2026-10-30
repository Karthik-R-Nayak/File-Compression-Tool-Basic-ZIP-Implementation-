#include "cli.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    bool expect(bool condition, const std::string& description)
    {
        if (!condition)
            std::cerr << "FAILED: " << description << std::endl;
        return condition;
    }

    bool testArgumentParsing()
    {
        CliOptions options;
        std::string errorMessage;

        const bool explicitOutput = parseCliArguments(
            std::vector<std::string>{
                "compress", "input.txt", "--output", "result.huff"
            },
            options,
            errorMessage);
        if (!expect(explicitOutput, "parse explicit output path"))
            return false;
        if (!expect(options.command == CliCommand::Compress &&
                        options.inputPath == "input.txt" &&
                        options.outputPath == "result.huff",
                    "store parsed compression arguments"))
            return false;

        const bool defaultOutput = parseCliArguments(
            std::vector<std::string>{"compress", "input.txt"},
            options,
            errorMessage);
        if (!expect(defaultOutput, "parse default output path"))
            return false;
        if (!expect(options.outputPath == "input.txt.huff",
                    "derive default output path"))
            return false;

        const bool rejectedMissingInput = !parseCliArguments(
            std::vector<std::string>{"compress"},
            options,
            errorMessage);
        if (!expect(rejectedMissingInput, "reject missing input path"))
            return false;

        const bool rejectedUnknownOption = !parseCliArguments(
            std::vector<std::string>{"compress", "input.txt", "--unknown"},
            options,
            errorMessage);
        if (!expect(rejectedUnknownOption, "reject unknown option"))
            return false;

        const bool rejectedDecompression = !parseCliArguments(
            std::vector<std::string>{"decompress", "input.huff"},
            options,
            errorMessage);
        return expect(rejectedDecompression &&
                          errorMessage.find("not available") !=
                              std::string::npos,
                      "report decompression as unavailable");
    }

    bool testCompressionCommand()
    {
        const std::string inputPath = "cli_test_input.txt";
        const std::string outputPath = "cli_test_output.huff";
        std::remove(inputPath.c_str());
        std::remove(outputPath.c_str());

        {
            std::ofstream input(inputPath.c_str(), std::ios::binary);
            input << "CLI test input";
            if (!input)
                return expect(false, "create CLI input fixture");
        }

        std::ostringstream output;
        std::ostringstream errors;
        const int result = runCli(
            std::vector<std::string>{
                "compress", inputPath, "-o", outputPath
            },
            output,
            errors);

        std::ifstream compressed(outputPath.c_str(), std::ios::binary);
        char magic[4] = {};
        compressed.read(magic, sizeof(magic));
        const bool validOutput =
            compressed.gcount() == 4 &&
            magic[0] == 'H' && magic[1] == 'U' &&
            magic[2] == 'F' && magic[3] == 'F';
        compressed.close();

        const std::string outputText = output.str();
        std::remove(inputPath.c_str());
        std::remove(outputPath.c_str());

        return expect(result == 0, "compress through CLI") &&
               expect(validOutput, "create a valid compressed output") &&
               expect(outputText.find("Compression successful.") !=
                          std::string::npos,
                      "display successful command status") &&
               expect(outputText.find("Original size: 14 bytes") !=
                          std::string::npos,
                      "display original size") &&
               expect(outputText.find("Compressed size: ") !=
                          std::string::npos,
                      "display compressed size") &&
               expect(outputText.find(
                          "Compression ratio (original/compressed): ") !=
                          std::string::npos,
                      "display compression ratio") &&
               expect(errors.str().empty(), "do not report CLI errors");
    }

    bool testEmptyInputStatistics()
    {
        const std::string inputPath = "cli_test_empty_input";
        const std::string outputPath = "cli_test_empty_output.huff";
        std::remove(inputPath.c_str());
        std::remove(outputPath.c_str());

        {
            std::ofstream input(inputPath.c_str(), std::ios::binary);
            if (!input)
                return expect(false, "create empty CLI input fixture");
        }

        std::ostringstream output;
        std::ostringstream errors;
        const int result = runCli(
            std::vector<std::string>{
                "compress", inputPath, "-o", outputPath
            },
            output,
            errors);

        const std::string outputText = output.str();
        std::remove(inputPath.c_str());
        std::remove(outputPath.c_str());

        return expect(result == 0, "compress empty input through CLI") &&
               expect(outputText.find("Original size: 0 bytes") !=
                          std::string::npos,
                      "display empty original size") &&
               expect(outputText.find("Compressed size: 23 bytes") !=
                          std::string::npos,
                      "display empty-input header size") &&
               expect(outputText.find(
                          "Compression ratio: N/A (empty input)") !=
                          std::string::npos,
                      "avoid division by zero for empty input") &&
               expect(errors.str().empty(),
                      "do not report errors for empty input");
    }
}

int main()
{
    if (!testArgumentParsing() || !testCompressionCommand() ||
        !testEmptyInputStatistics())
        return 1;

    std::cout << "All CLI tests passed." << std::endl;
    return 0;
}
