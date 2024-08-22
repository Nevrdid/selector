#ifndef SDL_FILE_CHOOSER
#define SDL_FILE_CHOOSER

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include "SDL_ttf.h"
#include "SDL_image.h"

#define FILECHOOSER_TITLE "Choose a file"

class FileChooser
{
private:
    std::vector<std::string> fileList;

    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *backgroundTexture;

    int chosenFileI{};
    std::string title;

    void getFileList(std::string directory, bool recursive);
    void drawFileList();
    void drawTitle(const std::string &title);
    void drawSelector();
    void drawBackground();
    void deinit();

public:
    FileChooser(std::string directory, std::string title, std::string backgroundImage = "", bool recursive = false);
    FileChooser(std::vector<std::string> customChoices, std::string title, std::string backgroundImage = "");  // New builder for customized choices
    std::string get();
};

#endif // SDL_FILE_CHOOSER
