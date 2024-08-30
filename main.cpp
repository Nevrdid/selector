#include "sdl_file_chooser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to replace all occurrences of \n with a real line feed
std::string replaceLiteralNewlines(const std::string &str)
{
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find("\\n", pos)) != std::string::npos)
    {
        result.replace(pos, 2, "\n");
        pos += 1; // Advance one position after replacement
    }
    return result;
}

int main(int argc, char *argv[])
{
    std::string directory = ".";
    std::string title = FILECHOOSER_TITLE;
    std::string backgroundImage;
    bool recursive = false;
    std::vector<std::string> customChoices;
    bool useCustomChoices = false;
    std::vector<std::string> filters; // Container for filters

    // Parse arguments
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc)
        {
            directory = argv[++i];
        }
        else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc)
        {
            if (strlen(argv[i + 1]) > 0)
            {
                title = replaceLiteralNewlines(argv[++i]);
            }
            else
            {
                std::cerr << "Error: Title cannot be empty. Using default title." << std::endl;
                title = " ";
            }
        }
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        {
            backgroundImage = argv[++i];
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            recursive = true;
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            useCustomChoices = true;
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                customChoices.push_back(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-cf") == 0)
        {
            // Collect choices from a file
            if (++i < argc)
            {
                std::ifstream inputFile(argv[i]);
                if (!inputFile)
                {
                    std::cerr << "Error: Cannot open file " << argv[i] << "\n";
                    return 1;
                }
                std::string line;
                while (std::getline(inputFile, line))
                {
                    if (!line.empty())
                    {
                        customChoices.push_back(line);
                    }
                }
                useCustomChoices = true; // Ensure custom choices are used
            }
            else
            {
                std::cerr << "Error: No file specified for -cf option\n";
                return 1;
            }
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            // Collect filters
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                filters.push_back(argv[++i]);
            }
        }
    }

    // Choose the correct FileChooser constructor based on the conditions
    FileChooser *fileChooser = nullptr;
    if (useCustomChoices)
    {
        fileChooser = new FileChooser(customChoices, title, backgroundImage);
    }
    else
    {
        fileChooser = new FileChooser(directory, title, backgroundImage, recursive, filters); // Pass filters to FileChooser
    }

    // Get the chosen file
    std::string chosenFile = fileChooser->get();

    if (!chosenFile.empty())
    {
        std::cout << "You selected: " << chosenFile << "\n";
    }
    else
    {
        std::cout << "No file selected\n";
    }

    delete fileChooser; // Clean up the dynamically allocated object

    return 0;
}
