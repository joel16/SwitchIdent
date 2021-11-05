#ifndef _SWITCHIDENT_GUI_H_
#define _SWITCHIDENT_GUI_H_

#include <switch.h>
#include <SDL2/SDL.h>

#include "SDL_FontCache.h"

#define BACKGROUND_COLOUR      FC_MakeColor(242, 241, 240, 255)
#define STATUS_BAR_COLOUR      FC_MakeColor(69, 67, 62, 255)
#define MENU_BAR_COLOUR        FC_MakeColor(255, 255, 255, 255)
#define ITEM_COLOUR            FC_MakeColor(0, 0, 0, 255)
#define ITEM_SELECTED_COLOUR   MENU_BAR_COLOUR
#define MENU_SELECTOR_COLOUR   FC_MakeColor(239, 118, 69, 255)
#define MENU_INFO_TITLE_COLOUR FC_MakeColor(144, 137, 129, 255)
#define MENU_INFO_DESC_COLOUR  FC_MakeColor(51, 51, 51, 255)

namespace GUI {
    int Init(void);
    void Exit(void);
    void ClearScreen(SDL_Color colour);
    void DrawRect(int x, int y, int w, int h, SDL_Color colour);
    void DrawText(int x, int y, int size, SDL_Color colour, const char *text);
    void DrawTextf(int x, int y, int size, SDL_Color colour, const char* text, ...);
    void GetTextDimensions(int size, const char *text, u32 *width, u32 *height);
    void DrawBanner(int x, int y);
    void DrawDriveIcon(int x, int y);
    void Render(void);
}

#endif
