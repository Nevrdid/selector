#include "sdl_file_chooser.h"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[])
{
    std::string directory = ".";
    std::string title = FILECHOOSER_TITLE;
    std::string backgroundImage;
    bool recursive = false;
    std::vector<std::string> customChoices;  // New container for -c option
    bool useCustomChoices = false;  // Flag to know if custom choices are used

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            directory = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            if (strlen(argv[i + 1]) > 0) {
                title = argv[++i];
            } else {
                std::cerr << "Error: Title cannot be empty. Using default title." << std::endl;
                title = " ";
            }
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            backgroundImage = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0) {
            recursive = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            useCustomChoices = true;
            // Retrieve all the following arguments as long as they are not options
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                customChoices.push_back(argv[++i]);
            }
        }
    }

    if (useCustomChoices) {
        FileChooser fileChooser{customChoices, title, backgroundImage};  // Builder with customized list
        std::string chosenFile{fileChooser.get()};
        std::cout << "Selected: " << chosenFile << '\n';
    } else {
        FileChooser fileChooser{directory, title, backgroundImage, recursive};
        std::string chosenFile{fileChooser.get()};
        std::cout << "Selected: " << chosenFile << '\n';
    }

    return 0;
}
