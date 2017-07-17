#include <string>
#include <iostream>

#include <filehelper.h>
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
    background = LoadImageWithBMPFormat(getFullPath("guest.bmp"), renderer);
    image = LoadImageWithBMPFormat(getFullPath("SakuraSmall2.bmp"), renderer);
    if (background == nullptr || image == nullptr)
        return 4;

    SDL_RenderClear(renderer);

    int bW, bH;
    SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
    ApplySurface(0, 0, background, renderer);

    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    ApplySurface(x, y, image, renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}