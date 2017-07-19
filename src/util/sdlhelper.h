#ifndef _SDLHELPER_H_
#define _SDLHELPER_H_

#include <iostream>

#include "SDL.h"

#ifdef SDL_image
#include "SDL_image.h"
#endif

#ifdef SDL_ttf
#include "SDL_ttf.h"
#endif

namespace SDL_helper
{

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

    void ApplySurface(int x, int y, int w, int h, SDL_Texture *tex, SDL_Renderer *rend)
    {
        SDL_Rect pos;
        pos.x = x;
        pos.y = y;
        pos.w = w;
        pos.h = h;
        SDL_RenderCopy(rend, tex, NULL, &pos);
    }


    template<typename T, typename... Args>
    void cleanup(T *t, Args&&... args) {
        //Cleanup the first item in the list
        cleanup(t);
        //Clean up the remaining arguments
        cleanup(std::forward<Args>(args)...);
    }
    /*
    * These specializations serve to free the passed argument and also provide the
    * base cases for the recursive call above, eg. when args is only a single
    * item one of the specializations below will be called by
    * cleanup(std::forward<Args>(args)...), ending the recursion
    */
    template<>
    inline void cleanup<SDL_Window>(SDL_Window *win) {
        if (!win) {
            return;
        }
        SDL_DestroyWindow(win);
    }
    template<>
    inline void cleanup<SDL_Renderer>(SDL_Renderer *ren) {
        if (!ren) {
            return;
        }
        SDL_DestroyRenderer(ren);
    }
    template<>
    inline void cleanup<SDL_Texture>(SDL_Texture *tex) {
        if (!tex) {
            return;
        }
        SDL_DestroyTexture(tex);
    }
    template<>
    inline void cleanup<SDL_Surface>(SDL_Surface *surf) {
        if (!surf) {
            return;
        }
        SDL_FreeSurface(surf);
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

};
#endif