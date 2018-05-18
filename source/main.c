#include <string.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <switch.h>

#include "kernel.h"
#include "misc.h"
#include "system.h"
#include "set.h"

static void SwitchIdent_InitServices(void)
{
	Result ret = 0;

	if (R_FAILED(ret = setInitialize()))
		printf("setInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = splInitialize()))
		printf("splInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = socketInitializeDefault()))
		printf("socketInitializeDefault() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = setsysInitialize2()))
		printf("setsysInitialize2() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = appletInitialize()))
		printf("appletInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = apmInitialize()))
		printf("apmInitialize() failed: 0x%x.\n\n", ret);
}

static void SwitchIdent_TermServices(void)
{
	apmExit();
	appletExit();
	setsysExit2();
	socketExit();
	splExit();
	setExit();
}

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(NULL);

	SwitchIdent_InitServices();
	
	Result ret = 0;
	char serial[0x18];

	printf("\x1b[1;1H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1mSwitchIdent 0.1\x1b[0m\n\n");

	printf("\x1b[31;1m*\x1b[0m System version: \x1b[31;1m%s\n", SwitchIdent_GetVersion());
	printf("\x1b[31;1m*\x1b[0m Hardware: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m) \x1b[0m\n", SwitchIdent_GetHardwareType(), SwitchIdent_GetUnit());

	if (R_FAILED(ret = setGetSerialNumber(serial)))
		printf("setGetSerialNumber() failed: 0x%x.\n\n", ret);
	else
		printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\n\n", serial);

	printf("\x1b[33;1m*\x1b[0m Language: \x1b[33;1m%s\n", SwitchIdent_GetLanguage());
	printf("\x1b[33;1m*\x1b[0m Region: \x1b[33;1m%s\n", SwitchIdent_GetRegion());
	printf("\x1b[33;1m*\x1b[0m CPU clock: \x1b[33;1m%lu\x1b[0m MHz\n", SwitchIdent_GetCPUClock());
	printf("\x1b[33;1m*\x1b[0m GPU clock: \x1b[33;1m%lu\x1b[0m MHz\n\n", SwitchIdent_GetGPUClock());

	printf("\x1b[36;1m*\x1b[0m IP: \x1b[36;1m%s\n", inet_ntoa(__nxlink_host));
	printf("\x1b[36;1m*\x1b[0m State: \x1b[36;1m%s\n\n", SwitchIdent_GetOperationMode());

	printf("\x1b[32;1m> Press the plus key to exit =)\x1b[0m");

	while(appletMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) 
			break; // break in order to return to hbmenu

		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();
	}

	SwitchIdent_TermServices();
	gfxExit();
	return 0;
}
