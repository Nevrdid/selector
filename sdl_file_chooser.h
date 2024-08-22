#ifndef SDL_FILE_CHOOSER
#define SDL_FILE_CHOOSER

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include "SDL_ttf.h"
#include "SDL_image.h"  // Include SDL_image

#define FILECHOOSER_TITLE "Choose a file"

class FileChooser
{
private:
    std::vector<std::string> fileList;

    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *backgroundTexture; // Texture for background image

    int chosenFileI{};
    std::string title;

    void getFileList(std::string directory);
    void drawFileList();
    void drawTitle(const std::string &title);
    void drawSelector();
    void drawBackground();  // How to draw the background image

    void deinit();

public:
    FileChooser(std::string directory, std::string title, std::string backgroundImage = "");
    std::string get();
};

#endif // SDL_FILE_CHOOSER
