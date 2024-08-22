#include "sdl_file_chooser.h"
#include <cmath>
#include <stdint.h>
#include <iostream>

void FileChooser::getFileList(std::string directory)
{
    auto files{std::filesystem::recursive_directory_iterator{
        directory,
        std::filesystem::directory_options::skip_permission_denied}};
    
    for (auto &file : files)
    {
        if (std::filesystem::is_regular_file(file))
            fileList.push_back(file.path().c_str());
    }
    
    std::sort(fileList.begin(), fileList.end());
}

void FileChooser::drawTitle(const std::string &title)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, title.c_str(), {255, 255, 255, 255});
    
    SDL_Rect sourceRect{0, 0, textSurface->w, textSurface->h};
    SDL_Rect targetRect{10, 10, textSurface->w / 3, textSurface->h / 3};
    
    SDL_Texture* textTexture{SDL_CreateTextureFromSurface(renderer, textSurface)};
    
    SDL_RenderCopy(renderer, textTexture, &sourceRect, &targetRect);
    
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void FileChooser::drawFileList()
{
    for (int i{}; i < static_cast<int>(fileList.size()); ++i)
    {
        int y{500-chosenFileI*30+i*30};
        
        if (y < 1000 && y > 0)
        {
            SDL_Surface *textSurface = TTF_RenderText_Blended(
                font,
                fileList[i].c_str(),
                {255, 255, 255, static_cast<uint8_t>(255-abs(500-y)/2)});
            
            SDL_Rect sourceRect{0, 0, textSurface->w, textSurface->h};
            SDL_Rect targetRect{0, y, textSurface->w / 5, textSurface->h / 5};
            
            SDL_Texture* textTexture{SDL_CreateTextureFromSurface(renderer, textSurface)};
            
            SDL_RenderCopy(renderer, textTexture, &sourceRect, &targetRect);
            
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
    }
}

void FileChooser::drawSelector()
{
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    SDL_Rect selectorRect{0, 500, 800, 25};
    SDL_RenderFillRect(renderer, &selectorRect);
}

FileChooser::FileChooser(std::string directory)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    TTF_Init();
    
    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 1000, SDL_WINDOW_ALLOW_HIGHDPI);
    
    if (!window)
    {
        std::cerr << "Unable to create window" << '\n';
        std::exit(2);
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    if (!renderer)
    {
        std::cerr << "Unable to create renderer" << '\n';
        std::exit(2);
    }
    
    font = TTF_OpenFont("./Anonymous_Pro.ttf", 100);
    
    if (!font)
    {
        std::cerr << "Unable to open font file." << '\n';
        std::exit(2);
    }

    SDL_GameController* controller = NULL;
    if (SDL_NumJoysticks() > 0) {
        controller = SDL_GameControllerOpen(0);
        if (!controller) {
            std::cerr << "Failed to open controller: " << SDL_GetError() << '\n';
        }
    } else {
        std::cerr << "No joystick found\n";
    }

    drawTitle("Loading...");
    SDL_RenderPresent(renderer);
    getFileList(directory);

bool isRunning{true};
while (isRunning)
{
    SDL_Event event;
    
    SDL_PollEvent(&event);
    
    switch (event.type)
    {
        case SDL_QUIT:
            chosenFileI = -1;
            isRunning = false;
            break;
            
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    chosenFileI = -1;
                    isRunning = false;
                    break;
                
                case SDLK_DOWN:
                    chosenFileI += 1;
                    if (chosenFileI > static_cast<int>(fileList.size())-1)
                        chosenFileI = fileList.size()-1;
                    break;
                
                case SDLK_UP:
                    chosenFileI -= 1;
                    if (chosenFileI < 0)
                        chosenFileI = 0;
                    break;
                
                case SDLK_RETURN:
                    isRunning = false;
                    deinit();
                    return;
            }
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    chosenFileI += 1;
                    if (chosenFileI > static_cast<int>(fileList.size())-1)
                        chosenFileI = fileList.size()-1;
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    chosenFileI -= 1;
                    if (chosenFileI < 0)
                        chosenFileI = 0;
                    break;

                case SDL_CONTROLLER_BUTTON_B:
                    isRunning = false;
                    deinit();
                    return;

                case SDL_CONTROLLER_BUTTON_A:
                    chosenFileI = -1;
                    isRunning = false;
                    break;
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            // Gestion du mouvement des sticks analogiques

            break;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawSelector();
    drawTitle(FILECHOOSER_TITLE);
    drawFileList();
    
    SDL_RenderPresent(renderer);
    
    SDL_Delay(20);
}

if (controller) {
    SDL_GameControllerClose(controller);
}

deinit();
}

std::string FileChooser::get()
{
    if (chosenFileI != -1)
        return fileList.at(chosenFileI);

    return ""; // return an empty string if the user closed the window
}

void FileChooser::deinit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    TTF_CloseFont(font);
    
    TTF_Quit();
    SDL_Quit();
}
