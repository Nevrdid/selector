#include "sdl_file_chooser.h"
#include <iostream>
#include <cstring>
#include <vector>

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
    std::vector<std::string> filters; // New container for filters

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
        else if (strcmp(argv[i], "-f") == 0)
        { // New -f option for filtering
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                filters.push_back(argv[++i]); // Collect filters
            }
        }
    }

    if (useCustomChoices)
    {
        FileChooser fileChooser{customChoices, title, backgroundImage};
        std::string chosenFile{fileChooser.get()};
        std::cout << "Selected: " << chosenFile << '\n';
    }
    else
    {
        FileChooser fileChooser{directory, title, backgroundImage, recursive, filters}; // Pass filters to FileChooser
        std::string chosenFile{fileChooser.get()};
        std::cout << "Selected: " << chosenFile << '\n';
    }

    return 0;
}
