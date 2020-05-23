#include <switch.h>

#include "menus.h"
#include "power.h"
#include "setcal.h"
#include "SDL_helper.h"
#include "wlan.h"

//#define DEBUG

static void Term_Services(void) {
	socketExit();
	wlaninfExit();
	pcvExit();
	psmExit();
	nsExit();
	apmExit();
	appletExit();
	clkrstExit();
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

	if(R_FAILED(ret = clkrstInitialize()))
		printf("clkrstInitialize() failed: 0x%x.\n\n", ret);

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

	if (R_FAILED(ret = wlaninfInitialize()))
		printf("wlaninfInitialize() failed: 0x%x.\n\n", ret);

	printf("Loaded services!\n");

	SDL_HelperInit();
}

int main(int argc, char **argv) {
#ifdef DEBUG
	socketInitializeDefault();
    nxlinkStdio();
	printf("Loading Services...\n");
#endif
	Init_Services();
	Menu_Main();
	Term_Services();
}
