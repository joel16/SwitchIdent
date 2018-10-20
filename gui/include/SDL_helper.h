#ifndef NX_SHELL_SDL_HELPER_H
#define NX_SHELL_SDL_HELPER_H

#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_FontCache.h"

#define BACKGROUND_COLOUR      FC_MakeColor(242, 241, 240, 255)
#define STATUS_BAR_COLOUR      FC_MakeColor(69, 67, 62, 255)
#define MENU_BAR_COLOUR        FC_MakeColor(255, 255, 255, 255)
#define ITEM_COLOUR            FC_MakeColor(0, 0, 0, 255)
#define ITEM_SELECTED_COLOUR   MENU_BAR_COLOUR
#define MENU_SELECTOR_COLOUR   FC_MakeColor(239, 118, 69, 255)
#define MENU_INFO_TITLE_COLOUR FC_MakeColor(144, 137, 129, 255)
#define MENU_INFO_DESC_COLOUR  FC_MakeColor(51, 51, 51, 255)

Result SDL_HelperInit(void);
void SDL_HelperTerm(void);
void SDL_ClearScreen(SDL_Color colour);
void SDL_DrawRect(int x, int y, int w, int h, SDL_Color colour);
void SDL_DrawText(int x, int y, int size, SDL_Color colour, const char *text);
void SDL_DrawTextf(int x, int y, int size, SDL_Color colour, const char* text, ...);
void SDL_GetTextDimensions(int size, const char *text, u32 *width, u32 *height);
void SDL_DrawBanner(int x, int y);
void SDL_DrawDriveIcon(int x, int y);
void SDL_Renderdisplay(void);

#endif
