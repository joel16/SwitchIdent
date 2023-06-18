#pragma once

#include <switch.h>
#include <SDL2/SDL.h>

extern SDL_Texture *banner, *drive, *menu_icons[7];

namespace GUI {
    int Init(void);
    void Exit(void);
    void ClearScreen(SDL_Color colour);
    void DrawRect(int x, int y, int w, int h, SDL_Color colour);
    void DrawText(int x, int y, int size, SDL_Color colour, const char *text);
    void DrawTextf(int x, int y, int size, SDL_Color colour, const char* text, ...);
    void GetTextDimensions(int size, const char *text, int *width, int *height);
    void DrawImage(SDL_Texture *texture, int x, int y);
    void Render(void);
}
