#ifndef SDL_FILE_CHOOSER
#define SDL_FILE_CHOOSER

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include "SDL_ttf.h"

#define FILECHOOSER_TITLE "Choose a file"

class FileChooser
{
private:
    std::vector<std::string> fileList;

    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    
    int chosenFileI{};
    std::string title; // Add a custom title field

    void getFileList(std::string directory);
    
    void drawFileList();
    void drawTitle(const std::string &title);
    void drawSelector();

    void deinit();

public:
    FileChooser(std::string directory, std::string customTitle = FILECHOOSER_TITLE); // Constructor with optional title
    
    std::string get();
};

#endif // SDL_FILE_CHOOSER
