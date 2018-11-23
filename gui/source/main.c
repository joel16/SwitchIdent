#include <switch.h>

#include "menus.h"
#include "SDL_helper.h"

static void Term_Services(void) {
	pcvExit();
	psmExit();
	nsExit();
	apmExit();
	appletExit();
	socketExit();
	nifmExit();
	splExit();
	setsysExit();
	setExit();
	SDL_HelperTerm();
	plExit();
	romfsExit();
}

static void Init_Services(void) {
	Result ret = 0;

	if (R_FAILED(ret = romfsInit()))
		printf("romfsInit() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = plInitialize()))
		printf("plInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = SDL_HelperInit()))
		printf("SDL_HelperInit() failed: 0x%x.\n\n", ret);

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

	if (R_FAILED(ret = psmInitialize()))
		printf("psmInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = pcvInitialize()))
		printf("pcvInitialize() failed: 0x%x.\n\n", ret);
}

int main(int argc, char **argv) {
	Init_Services();
	Menu_Main();
	Term_Services();
}