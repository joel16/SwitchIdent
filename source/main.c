#include <switch.h>

#include "menus.h"
#include "power.h"
#include "SDL_helper.h"
#include "wlan.h"

static void Term_Services(void) {
	wlaninfExit();
	if (hosversionAtLeast(8, 0, 0))
		clkrstExit();
	else
		pcvExit();
	psmExit();
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
	romfsExit();
}

static void Init_Services(void) {
	Result ret = 0;

	if (R_FAILED(ret = romfsInit()))
		printf("romfsInit() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = setInitialize()))
		printf("setInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = setsysInitialize()))
		printf("setsysInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = setcalInitialize()))
		printf("setcalInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = splInitialize()))
		printf("splInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = nifmInitialize(NifmServiceType_User)))
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

	if (hosversionAtLeast(8, 0, 0)) {
		if (R_FAILED(ret = clkrstInitialize()))
			printf("clkrstInitialize() failed: 0x%x.\n\n", ret);
	} else {
		if (R_FAILED(ret = pcvInitialize()))
			printf("pcvInitialize() failed: 0x%x.\n\n", ret);
	}

	if (R_FAILED(ret = wlaninfInitialize()))
		printf("wlaninfInitialize() failed: 0x%x.\n\n", ret);

	SDL_HelperInit();
}

int main(int argc, char **argv) {
	Init_Services();
	Menu_Main();
	Term_Services();
}
