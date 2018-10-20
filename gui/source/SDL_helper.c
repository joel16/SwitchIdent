#include "SDL_helper.h"

static SDL_Window *WINDOW;
static SDL_Renderer *RENDERER;
static FC_Font *font;
static PlFontData fontData, fontExtData;
static SDL_Texture *banner, *drive;

static FC_Font *GetFont(int size) {
	return font;
}

static void SDL_LoadImage(SDL_Texture **texture, char *path) {
	SDL_Surface *loaded_surface = NULL;
	loaded_surface = IMG_Load(path);

	if (loaded_surface) {
		Uint32 colorkey = SDL_MapRGB(loaded_surface->format, 0, 0, 0);
		SDL_SetColorKey(loaded_surface, SDL_TRUE, colorkey);
		*texture = SDL_CreateTextureFromSurface(RENDERER, loaded_surface);
	}

	SDL_FreeSurface(loaded_surface);
}

Result SDL_HelperInit(void) {
	Result ret = 0;

	SDL_Init(SDL_INIT_EVERYTHING);
	WINDOW = SDL_CreateWindow("SwitchIdent", 0, 0, 1280, 720, SDL_WINDOW_FULLSCREEN);
	RENDERER = SDL_CreateRenderer(WINDOW, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(RENDERER, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	IMG_Init(IMG_INIT_PNG);

	SDL_LoadImage(&banner, "romfs:/banner.png");
	SDL_LoadImage(&drive, "romfs:/drive.png");

	if (R_FAILED(ret = plGetSharedFontByType(&fontData, PlSharedFontType_Standard)))
		return ret;

	if (R_FAILED(ret = plGetSharedFontByType(&fontExtData, PlSharedFontType_NintendoExt)))
		return ret;

	font = FC_CreateFont();
	FC_LoadFont_RW(font, RENDERER, SDL_RWFromMem((void*)fontData.address, fontData.size), SDL_RWFromMem((void*)fontExtData.address, fontExtData.size), 1, 25, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

	return 0;
}

void SDL_HelperTerm(void) {
	FC_FreeFont(font);
	TTF_Quit();

	IMG_Quit();

	SDL_DestroyRenderer(RENDERER);
	SDL_DestroyWindow(WINDOW);
	SDL_Quit();
}

void SDL_ClearScreen(SDL_Color colour) {
	SDL_SetRenderDrawColor(RENDERER, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderClear(RENDERER);
}

void SDL_DrawRect(int x, int y, int w, int h, SDL_Color colour) {
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(RENDERER, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(RENDERER, &rect);
}

void SDL_DrawText(int x, int y, int size, SDL_Color colour, const char *text) {
	FC_DrawColor(GetFont(size), RENDERER, x, y, colour, text);
}

void SDL_DrawTextf(int x, int y, int size, SDL_Color colour, const char* text, ...) {
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	SDL_DrawText(x, y, size, colour, buffer);
	va_end(args);
}

void SDL_GetTextDimensions(int size, const char *text, u32 *width, u32 *height) {
	FC_Font *font = GetFont(size);

	if (width != NULL) 
		*width = FC_GetWidth(font, text);
	if (height != NULL) 
		*height = FC_GetHeight(font, text);
}

static void SDL_DrawImage(SDL_Texture *texture, int x, int y) {
	SDL_Rect position;
	position.x = x; position.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
	SDL_RenderCopy(RENDERER, texture, NULL, &position);
}

void SDL_DrawBanner(int x, int y) {
	SDL_DrawImage(banner, x, y);
}

void SDL_DrawDriveIcon(int x, int y) {
	SDL_DrawImage(drive, x, y);
}

void SDL_Renderdisplay(void) {
	SDL_RenderPresent(RENDERER);
}
