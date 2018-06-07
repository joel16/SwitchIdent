#ifndef _SWITCHIDENT_SDL_HELPER_H_
#define _SWITCHIDENT_SDL_HELPER_H_

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>

static inline SDL_Color SDL_MakeColour(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color colour = {r, g, b};
	return colour;
}

#define BACKGROUND_COLOUR      SDL_MakeColour(242, 241, 240)
#define STATUS_BAR_COLOUR      SDL_MakeColour(69, 67, 62)
#define MENU_BAR_COLOUR        SDL_MakeColour(255, 255, 255)
#define ITEM_COLOUR            SDL_MakeColour(0, 0, 0)
#define ITEM_SELECTED_COLOUR   MENU_BAR_COLOUR
#define MENU_SELECTOR_COLOUR   SDL_MakeColour(239, 118, 69)
#define MENU_INFO_TITLE_COLOUR SDL_MakeColour(144, 137, 129)
#define MENU_INFO_DESC_COLOUR  SDL_MakeColour(51, 51, 51)

SDL_Window *WINDOW;
SDL_Surface *WINDOW_SURFACE;
SDL_Renderer *RENDERER;
SDL_Texture *banner;
TTF_Font *Ubuntu_R_large, *Ubuntu_R;

void SDL_ClearScreen(SDL_Renderer* renderer, SDL_Color colour);
void SDL_DrawRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color colour);
void SDL_DrawText(TTF_Font *font, int x, int y, SDL_Color colour, const char *text);
SDL_Surface *SDL_LoadImage(SDL_Renderer* renderer, SDL_Texture **texture, char *path);
void SDL_DrawImage(SDL_Renderer* renderer, SDL_Texture *texture, int x, int y, int w, int h);

#endif