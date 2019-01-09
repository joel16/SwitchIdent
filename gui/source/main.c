#include <switch.h>

#include "menus.h"
#include "power.h"
#include "setcal.h"
#include "SDL_helper.h"
#include "wlan.h"

static void Term_Services(void) {
	wlaninfExit();
	pcvExit();
	powerExit();
	nsExit();
	apmExit();
	appletExit();
	socketExit();
	nifmExit();
	splExit();
	setcalExit();
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

	if (R_FAILED(ret = setcalInitialize()))
		printf("setcalInitialize() failed: 0x%x.\n\n", ret);

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

	if (R_FAILED(ret = powerInitialize()))
		printf("powerInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = pcvInitialize()))
		printf("pcvInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = wlaninfInitialize()))
		printf("wlaninfInitialize() failed: 0x%x.\n\n", ret);
}

int main(int argc, char **argv) {
	Init_Services();
	Menu_Main();
	Term_Services();
}
