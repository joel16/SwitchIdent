#include <switch.h>

#include "menus.h"
#include "SDL_helper.h"

static void Term_Services(void)
{
	nsExit();
	apmExit();
	appletExit();
	socketExit();
	nifmExit();
	splExit();
	setsysExit();
	setExit();

	TTF_CloseFont(Ubuntu_R_large);
	TTF_CloseFont(Ubuntu_R);
	TTF_Quit();

	SDL_DestroyTexture(banner);

	IMG_Quit();

	SDL_DestroyRenderer(RENDERER);
	SDL_FreeSurface(WINDOW_SURFACE);
	SDL_DestroyWindow(WINDOW);

	SDL_Quit();
	romfsExit();
}

static void Init_Services(void)
{
	romfsInit();
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_CreateWindowAndRenderer(1280, 720, 0, &WINDOW, &RENDERER);

	WINDOW_SURFACE = SDL_GetWindowSurface(WINDOW);

	SDL_SetRenderDrawBlendMode(RENDERER, SDL_BLENDMODE_BLEND);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	IMG_Init(IMG_INIT_PNG);

	SDL_LoadImage(RENDERER, &banner, "romfs:/banner.png");

	TTF_Init();
	
	Ubuntu_R = TTF_OpenFont("romfs:/Ubuntu-R.ttf", 25);
	Ubuntu_R_large = TTF_OpenFont("romfs:/Ubuntu-R.ttf", 30);
	
	if (!Ubuntu_R || !Ubuntu_R_large)
		Term_Services();

	Result ret = 0;

	if (R_FAILED(ret = setInitialize()))
		printf("setInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = setsysInitialize()))
		printf("setsysInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = splInitialize()))
		printf("splInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = nifmInitialize()))
		printf("nifmInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = socketInitializeDefault()))
		printf("socketInitializeDefault() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = appletInitialize()))
		printf("appletInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = apmInitialize()))
		printf("apmInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = nsInitialize()))
		printf("nsInitialize() failed: 0x%x.\n\n", ret);
}

int main(int argc, char **argv)
{
	Init_Services();
	Menu_Main();
	Term_Services();
}