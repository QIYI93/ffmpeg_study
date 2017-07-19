#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

using std::string;
class SDLWindow
{
public:
    SDLWindow();

    void Init(string title = "Window");
    void Quit();
    void Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip = NULL, float angle = 0.0, int xPivot = 0, int yPivot = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    SDL_Texture* loadImage(const string &file);
    SDL_Texture* RenderText(const string &message, const string &fontFile, SDL_Color color, int fontSize);
    void Clear();
    void Present();
    SDL_Rect Box();

    ~SDLWindow();

private:
     std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mWindow;
     std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> mRenderer;
     SDL_Rect mBox;
};



#endif // SDLWINDOW_H