#include <stdexcept>
#include <string>
#include <iostream>

#include <filehelper.h>
#define SDL_image
#include <sdlhelper.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char** argv)
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return 2;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
        | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return 3;
    }

    SDL_Texture *background = nullptr, *image = nullptr;
    try
    {
        background = SDL_helper::LoadImage(getFullPath("guest.bmp"), renderer);
        image = SDL_helper::LoadImage(getFullPath("image_1.bmp"), renderer);
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 4;
    }

    SDL_RenderClear(renderer);
    int bW, bH;
    SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
    SDL_helper::ApplySurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, background, renderer);

    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;

    //Our event structure
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }
        SDL_helper::ApplySurface(x, y, iW, iH, image, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_helper::cleanup(image, background, renderer, window);
    SDL_Quit();

    return 0;
}