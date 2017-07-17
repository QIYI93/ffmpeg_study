#ifndef _SDLHELPER_H_
#define _SDLHELPER_H_

#include <iostream>

#include "SDL.h"

#ifdef SDL_image
#include "SDL_image.h"
#endif

using std::string;
SDL_Texture* LoadImageWithBMPFormat(string file, SDL_Renderer* renderer)
{
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;

    loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    else
        std::cout << SDL_GetError() << std::endl;
    return texture;
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);

    SDL_RenderCopy(rend, tex, NULL, &pos);
}

#ifdef SDL_image
SDL_Texture* LoadImage(string file, SDL_Renderer* renderer) {
    SDL_Texture* tex = nullptr;
    tex = IMG_LoadTexture(renderer, file.c_str());
    if (tex == nullptr)
        throw std::runtime_error("Failed to load image: " + file + IMG_GetError());
    return tex;
}
#endif


#endif