#include "sdlwindow.h"



SDLWindow::SDLWindow()
:mWindow(nullptr, SDL_DestroyWindow)
,mRenderer(nullptr, SDL_DestroyRenderer)
{
}

SDLWindow::~SDLWindow()
{
}

void SDLWindow::Init(string title /* = "Window" */)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        throw std::runtime_error("SDL Init Failed");
    if (TTF_Init() == -1)
        throw std::runtime_error("TTF Init Failed");

    mBox.x = 0;
    mBox.y = 0;
    mBox.w = 640;
    mBox.h = 480;

    mWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, mBox.w, mBox.h, SDL_WINDOW_SHOWN));

    if (mWindow == nullptr)
        throw std::runtime_error("Failed to create window");

    mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if (mRenderer == nullptr)
        throw std::runtime_error("Failed to create renderer");
}

void SDLWindow::Quit() {
    TTF_Quit();
    SDL_Quit();
}


void SDLWindow::Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip)
{
    xPivot += dstRect.w / 2;
    yPivot += dstRect.h / 2;

    SDL_Point pivot = { xPivot, yPivot };
    SDL_RenderCopyEx(mRenderer.get(), tex, clip, &dstRect, angle, &pivot, flip);
}

SDL_Texture* SDLWindow::RenderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize)
{
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr)
        throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());

    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);

    return texture;
}

void SDLWindow::Clear()
{
    SDL_RenderClear(mRenderer.get());
}
void SDLWindow::Present()
{
    SDL_RenderPresent(mRenderer.get());
}
SDL_Rect SDLWindow::Box()
{
    SDL_GetWindowSize(mWindow.get(), &mBox.w, &mBox.h);
    return mBox;
}

SDL_Texture* SDLWindow::loadImage(const std::string& file)
{

    SDL_Texture* tex = nullptr;
    tex = IMG_LoadTexture(mRenderer.get(), file.c_str());
    if (tex == nullptr)
        throw std::runtime_error("Failed to load image: " + file + IMG_GetError());
    return tex;
}