#include "cli.hpp"

#include "encoder.hpp"
#include "file_reader.hpp"
#include "frequency.hpp"
#include "huffman_tree.hpp"

#include <exception>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace
{
    bool startsWithDash(const std::string& value)
    {
        return !value.empty() && value[0] == '-';
    }

    int runCompression(const CliOptions& options,
                       std::ostream& output,
                       std::ostream& errors)
    {
        std::vector<std::uint8_t> data;
        if (!readInputFile(options.inputPath, data))
            return 1;

        try
        {
            const FrequencyTable frequencies = calculateFrequency(data);
            const std::shared_ptr<HuffmanNode> tree =
                buildHuffmanTree(frequencies);
            const HuffmanCodeMap generatedCodes =
                generateHuffmanCodes(tree);
            const std::unordered_map<std::uint8_t, std::string> codes(
                generatedCodes.begin(), generatedCodes.end());

            if (!writeHuffFile(data, codes, options.outputPath))
                return 1;
        }
        catch (const std::exception& error)
        {
            errors << "Error: Compression failed: " << error.what()
                   << std::endl;
            return 1;
        }

        output << "Compression successful." << std::endl;
        output << "Output: " << options.outputPath << std::endl;
        return 0;
    }
}

bool parseCliArguments(const std::vector<std::string>& arguments,
                       CliOptions& options,
                       std::string& errorMessage)
{
    options = CliOptions();
    errorMessage.clear();

    if (arguments.empty() || arguments[0] == "--help" ||
        arguments[0] == "-h")
    {
        options.command = CliCommand::Help;
        return true;
    }

    if (arguments[0] != "compress")
    {
        if (arguments[0] == "decompress")
        {
            errorMessage =
                "Decompression is not available yet; it depends on "
                "FCT-F-010 through FCT-F-014.";
        }
        else
        {
            errorMessage = "Unknown command: " + arguments[0];
        }
        return false;
    }

    options.command = CliCommand::Compress;
    bool outputPathProvided = false;
    bool parseOptions = true;
    std::vector<std::string> positionalArguments;

    for (std::size_t i = 1; i < arguments.size(); ++i)
    {
        const std::string& argument = arguments[i];
        if (parseOptions && argument == "--")
        {
            parseOptions = false;
        }
        else if (parseOptions &&
                 (argument == "-o" || argument == "--output"))
        {
            if (outputPathProvided)
            {
                errorMessage = "Specify the output path only once.";
                return false;
            }
            if (i + 1 >= arguments.size())
            {
                errorMessage = "Missing path after " + argument + ".";
                return false;
            }

            options.outputPath = arguments[++i];
            if (options.outputPath.empty())
            {
                errorMessage = "The output path cannot be empty.";
                return false;
            }
            outputPathProvided = true;
        }
        else if (parseOptions && startsWithDash(argument))
        {
            errorMessage = "Unknown option: " + argument;
            return false;
        }
        else
        {
            positionalArguments.push_back(argument);
        }
    }

    if (positionalArguments.empty())
    {
        errorMessage = "The compress command requires an input file.";
        return false;
    }
    if (positionalArguments.size() > 1)
    {
        errorMessage = "Specify exactly one input file.";
        return false;
    }

    options.inputPath = positionalArguments[0];
    if (!outputPathProvided)
        options.outputPath = options.inputPath + ".huff";

    return true;
}

void printCliUsage(std::ostream& output)
{
    output << "Usage:\n"
           << "  file-compressor compress <input> "
              "[-o|--output <path>]\n"
           << "  file-compressor --help\n\n"
           << "If no output path is supplied, <input>.huff is used.\n"
           << "Decompression is not available in this version."
           << std::endl;
}

int runCli(const std::vector<std::string>& arguments,
           std::ostream& output,
           std::ostream& errors)
{
    CliOptions options;
    std::string errorMessage;
    if (!parseCliArguments(arguments, options, errorMessage))
    {
        errors << "Error: " << errorMessage << std::endl;
        printCliUsage(errors);
        return 2;
    }

    if (options.command == CliCommand::Help)
    {
        printCliUsage(output);
        return 0;
    }

    return runCompression(options, output, errors);
}
