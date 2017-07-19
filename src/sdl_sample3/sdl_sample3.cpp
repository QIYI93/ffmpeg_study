
#include "filehelper.h"
#define SDL_image
#define SDL_ttf
#include <sdlhelper.h>
#include "sdlwindow.h"


int main(int argc, char** argv)
{
    SDLWindow window;
    window.Init("Window");

    SDL_Texture *img, *msg;
    string filePath(file_Helper::getFullPath("image_1.bmp"));
    img = window.loadImage(filePath);
    string text = "Hello World";
    SDL_Color color = { 128, 128, 128 };
    msg = window.RenderText(text, file_Helper::getFullPath("SourceSansPro-Regular.ttf"), color, 80);
    int iW, iH;
    SDL_QueryTexture(img, NULL, NULL, &iW, &iH);
    SDL_Rect pos = { window.Box().w / 2 - iW / 2,
        window.Box().h / 2 - iH / 2, iW, iH };
    int angle = 0;

    window.Clear();

    window.Draw(img, pos, NULL, angle);
    window.Draw(msg, pos, NULL, angle, 0, 0);
    window.Present();
    SDL_Delay(2000);

    return 0;
}