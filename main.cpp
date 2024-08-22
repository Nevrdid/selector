#include "sdl_file_chooser.h"
#include <iostream>
#include <cstring>

int main(int argc, char *argv[])
{
    std::string title = "Choose a file";
    std::string backgroundImage = "";
    bool recursive = false;

    // Argument analysis
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            title = argv[++i];  // Customized title
        }
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            backgroundImage = argv[++i];  // Background image
        }
        else if (strcmp(argv[i], "-r") == 0) {
            recursive = true;  // Recursion enabled
        }
    }

    FileChooser fileChooser(".", title, backgroundImage, recursive);
    std::string chosenFile = fileChooser.get();
    
    std::cout << "Selected: " << chosenFile << '\n';
    
    return 0;
}
