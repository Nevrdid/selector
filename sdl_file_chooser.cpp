#include "sdl_file_chooser.h"
#include <cmath>
#include <stdint.h>
#include <iostream>

void FileChooser::getFileList(std::string directory, bool recursive)
{
    if (recursive)
    {
        auto files{std::filesystem::recursive_directory_iterator{
            directory,
            std::filesystem::directory_options::skip_permission_denied}};
        
        for (auto &file : files)
        {
            if (std::filesystem::is_regular_file(file))
                fileList.push_back(file.path().c_str());
        }
    }
    else
    {
        auto files{std::filesystem::directory_iterator{
            directory,
            std::filesystem::directory_options::skip_permission_denied}};
        
        for (auto &file : files)
        {
            if (std::filesystem::is_regular_file(file))
                fileList.push_back(file.path().c_str());
        }
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

void FileChooser::drawBackground()
{
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // Full-window image
    }
}
// New builder for customized selections with D-Pad event management
FileChooser::FileChooser(std::vector<std::string> customChoices, std::string title, std::string backgroundImage)
    : window(nullptr), renderer(nullptr), font(nullptr), backgroundTexture(nullptr), chosenFileI(0), title(title)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    TTF_Init();

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 1000, SDL_WINDOW_ALLOW_HIGHDPI);
    
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

    // Load background image if supplied
    if (!backgroundImage.empty()) {
        SDL_Surface* bgSurface = IMG_Load(backgroundImage.c_str());
        if (bgSurface) {
            backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
            SDL_FreeSurface(bgSurface);
        } else {
            std::cerr << "Failed to load background image: " << IMG_GetError() << '\n';
        }
    }

    font = TTF_OpenFont("./Anonymous_Pro.ttf", 100);
    
    if (!font)
    {
        std::cerr << "Unable to open font file." << '\n';
        std::exit(2);
    }

    fileList = customChoices;  // Use the custom choices provided

    // Controller opening if available
    SDL_GameController* controller = NULL;
    if (SDL_NumJoysticks() > 0) {
        controller = SDL_GameControllerOpen(0);
        if (!controller) {
            std::cerr << "Failed to open controller: " << SDL_GetError() << '\n';
        }
    } else {
        std::cerr << "No joystick found\n";
    }

    bool isRunning{true};
    while (isRunning)
    {
        SDL_Event event;
        
        while (SDL_PollEvent(&event))
        {
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

                        case SDL_CONTROLLER_BUTTON_A:
                            chosenFileI = -1;
                            isRunning = false;
                            deinit();
                            return;

                        case SDL_CONTROLLER_BUTTON_B:
                            isRunning = false;
                            deinit();
                            return;
                    }
                    break;

                case SDL_CONTROLLERAXISMOTION:
                    break;  // Completely ignore analog stick input
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBackground();
        drawSelector();
        drawTitle(title);
        drawFileList();
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(20);
    }
    
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }

    if (controller) {
        SDL_GameControllerClose(controller);
    }

    deinit();
}

FileChooser::FileChooser(std::string directory, std::string title, std::string backgroundImage, bool recursive)
    : window(nullptr), renderer(nullptr), font(nullptr), backgroundTexture(nullptr), chosenFileI(0), title(title)  // Initialiser dans le bon ordre
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    TTF_Init();
    
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 1000, SDL_WINDOW_ALLOW_HIGHDPI);
    
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

    // Load background image if supplied
    if (!backgroundImage.empty()) {
        SDL_Surface* bgSurface = IMG_Load(backgroundImage.c_str());
        if (bgSurface) {
            backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
            SDL_FreeSurface(bgSurface);
        } else {
            std::cerr << "Failed to load background image: " << IMG_GetError() << '\n';
        }
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
    getFileList(directory, recursive);  // Pass the recursive option here

    bool isRunning{true};
    while (isRunning)
    {
        SDL_Event event;
        
        while (SDL_PollEvent(&event))
        {
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
                    break;  // completely ignore analog stick input
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBackground();  // Draw background image
        drawSelector();
        drawTitle(title);
        drawFileList();
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(20);
    }
    
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }

    if (controller) {
        SDL_GameControllerClose(controller);
    }
}

std::string FileChooser::get()
{
    if (chosenFileI < 0)
        return "";
    return fileList[chosenFileI];
}

void FileChooser::deinit()
{
    if (font)
        TTF_CloseFont(font);

    if (renderer)
        SDL_DestroyRenderer(renderer);

    if (window)
        SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}
