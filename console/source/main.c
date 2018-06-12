#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <switch.h>

#include "kernel.h"
#include "misc.h"
#include "storage.h"
#include "system.h"
#include "utils.h"

static Service setsys_service;

static void SwitchIdent_InitServices(void)
{
	Result ret = 0;

	if (R_FAILED(ret = setInitialize()))
		printf("setInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = setsysInitialize()))
		printf("setsysInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = smGetService(&setsys_service, "set:sys")))
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

static void SwitchIdent_TermServices(void)
{
	nsExit();
	apmExit();
	appletExit();
	socketExit();
	nifmExit();
	splExit();
	serviceClose(&setsys_service);
	setsysExit();
	setExit();
}

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(NULL);

	SwitchIdent_InitServices();

	printf("\x1b[1;1H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1mSwitchIdent %d.%d %s\x1b[0m\n\n", VERSION_MAJOR, VERSION_MINOR, SwitchIdent_IsSafeMode()? "(SafeMode)" : "");

	/*
		Kernel/Hardware info:
	*/
	printf("\x1b[31;1m*\x1b[0m Firmware version: \x1b[31;1m%s\n", SwitchIdent_GetFirmwareVersion(&setsys_service));
	printf("\x1b[31;1m*\x1b[0m Kernel version: \x1b[31;1m%s\n", SwitchIdent_GetKernelVersion());
	printf("\x1b[31;1m*\x1b[0m Hardware: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m) \x1b[0m\n", SwitchIdent_GetHardwareType(), SwitchIdent_GetUnit());
	printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\n", SwitchIdent_GetSerialNumber());
	printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%llu\n", SwitchIdent_GetDeviceID());
	printf("\x1b[31;1m*\x1b[0m DRAM ID: \x1b[31;1m%s\n\n", SwitchIdent_GetDramDesc());

	/*
		System info:
	*/
	printf("\x1b[33;1m*\x1b[0m Region: \x1b[33;1m%s\n", SwitchIdent_GetRegion());
	printf("\x1b[33;1m*\x1b[0m CPU clock: \x1b[33;1m%lu\x1b[0m MHz\n", SwitchIdent_GetCPUClock());
	printf("\x1b[33;1m*\x1b[0m GPU clock: \x1b[33;1m%lu\x1b[0m MHz\n\n", SwitchIdent_GetGPUClock());

	/*
		Misc info:
	*/
	char hostname[128];
	Result ret = gethostname(hostname, sizeof(hostname));
	printf("\x1b[36;1m*\x1b[0m IP: \x1b[36;1m%s\n", R_SUCCEEDED(ret)? hostname : NULL);
	printf("\x1b[36;1m*\x1b[0m State: \x1b[36;1m%s\n", SwitchIdent_GetOperationMode());
	printf("\x1b[36;1m*\x1b[0m Wireless LAN: \x1b[36;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_WirelessLanEnable)? "Enabled" : "Disabled");
	printf("\x1b[36;1m*\x1b[0m Bluetooth: \x1b[36;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_BluetoothEnable)? "Enabled" : "Disabled");
	printf("\x1b[36;1m*\x1b[0m NFC: \x1b[36;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_NfcEnable)? "Enabled" : "Disabled");
	printf("\x1b[36;1m*\x1b[0m Automatic update: \x1b[36;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_AutoUpdateEnable)? "Enabled" : "Disabled");
	printf("\x1b[36;1m*\x1b[0m Console information upload: \x1b[36;1m%s\n\n", SwitchIdent_GetFlag(SetSysFlag_ConsoleInformationUpload)? "Enabled" : "Disabled");
	
	char sd_total_str[16], sd_free_str[16], sd_used_str[16];
	Utils_GetSizeString(sd_total_str, SwitchIdent_GetTotalStorage(FsStorageId_SdCard));
	Utils_GetSizeString(sd_free_str, SwitchIdent_GetFreeStorage(FsStorageId_SdCard));
	Utils_GetSizeString(sd_used_str, SwitchIdent_GetUsedStorage(FsStorageId_SdCard));

	char nand_u_total_str[16], nand_u_free_str[16], nand_u_used_str[16];
	Utils_GetSizeString(nand_u_total_str, SwitchIdent_GetTotalStorage(FsStorageId_NandUser));
	Utils_GetSizeString(nand_u_free_str, SwitchIdent_GetFreeStorage(FsStorageId_NandUser));
	Utils_GetSizeString(nand_u_used_str, SwitchIdent_GetUsedStorage(FsStorageId_NandUser));
	
	char nand_s_total_str[16], nand_s_free_str[16], nand_s_used_str[16];
	Utils_GetSizeString(nand_s_total_str, SwitchIdent_GetTotalStorage(FsStorageId_NandSystem));
	Utils_GetSizeString(nand_s_free_str, SwitchIdent_GetFreeStorage(FsStorageId_NandSystem));
	Utils_GetSizeString(nand_s_used_str, SwitchIdent_GetUsedStorage(FsStorageId_NandSystem));

	printf("\x1b[35;1m*\x1b[0m Total SD Capacity: \x1b[35;1m%s\n", sd_total_str);
	printf("\x1b[35;1m*\x1b[0m Free SD Capacity: \x1b[35;1m%s\n", sd_free_str);
	printf("\x1b[35;1m*\x1b[0m Used storage: \x1b[35;1m%s\n", sd_used_str);

	printf("\x1b[35;1m*\x1b[0m Total NAND (user) Capacity: \x1b[35;1m%s\n", nand_u_total_str);
	printf("\x1b[35;1m*\x1b[0m Free NAND (user) Capacity: \x1b[35;1m%s\n", nand_u_free_str);
	printf("\x1b[35;1m*\x1b[0m Used NAND (user) Capacity: \x1b[35;1m%s\n", nand_u_used_str);

	printf("\x1b[35;1m*\x1b[0m Total NAND (system) Capacity: \x1b[35;1m%s\n", nand_s_total_str);
	printf("\x1b[35;1m*\x1b[0m Free NAND (system) Capacity: \x1b[35;1m%s\n", nand_s_free_str);
	printf("\x1b[35;1m*\x1b[0m Used NAND (system) Capacity: \x1b[35;1m%s\n\n", nand_s_used_str);

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
