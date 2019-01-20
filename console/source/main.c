#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <switch.h>

#include "kernel.h"
#include "misc.h"
#include "power.h"
#include "storage.h"
#include "setcal.h"
#include "system.h"
#include "utils.h"
#include "wlan.h"

static bool isSDInserted = false, isGameCardInserted = false;

static void SwitchIdent_InitServices(void) {
	Result ret = 0;

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

	if (R_FAILED(ret = powerInitialize()))
		printf("powerInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = pcvInitialize()))
		printf("pcvInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = wlaninfInitialize()))
		printf("wlaninfInitialize() failed: 0x%x.\n\n", ret);

	FsDeviceOperator fsDeviceOperator;
	if (R_FAILED(ret = fsOpenDeviceOperator(&fsDeviceOperator)))
		printf("fsOpenDeviceOperator() failed: 0x%x.\n\n", ret);

	isSDInserted = SwitchIdent_IsSDCardInserted(&fsDeviceOperator);
	isGameCardInserted = SwitchIdent_IsGameCardInserted(&fsDeviceOperator);

	fsDeviceOperatorClose(&fsDeviceOperator);
}

static void SwitchIdent_TermServices(void) {
	wlaninfExit();
	pcvExit();
	powerExit();
	apmExit();
	appletExit();
	socketExit();
	nifmExit();
	splExit();
	setcalExit();
	setsysExit();
	setExit();
}

int main(int argc, char **argv) {
	consoleInit(NULL);

	SwitchIdent_InitServices();

	printf("\x1b[1;1H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1mSwitchIdent v%d.%d %s\x1b[0m\n\n", VERSION_MAJOR, VERSION_MINOR, SwitchIdent_IsSafeMode()? "(SafeMode)" : "");

	/*
		Kernel/Hardware info:
	*/
	printf("\x1b[31;1m*\x1b[0m Firmware version: \x1b[31;1m%s\n", SwitchIdent_GetFirmwareVersion());
	printf("\x1b[31;1m*\x1b[0m Hardware: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m) \x1b[0m\n", SwitchIdent_GetHardwareType(), SwitchIdent_GetUnit());
	printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\n", SwitchIdent_GetSerialNumber());
	printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%lu\n", SwitchIdent_GetDeviceID());
	printf("\x1b[31;1m*\x1b[0m DRAM ID: \x1b[31;1m%s\n\n", SwitchIdent_GetDramDesc());

	/*
		System info:
	*/
	printf("\x1b[33;1m*\x1b[0m Region: \x1b[33;1m%s\n", SwitchIdent_GetRegion());

	printf("\x1b[14;0H");
	printf("\x1b[33;1m*\x1b[0m Bluetooth MAC address: \x1b[33;1m%s\n", SwitchIdent_GetBluetoothBdAddress());
	printf("\x1b[33;1m*\x1b[0m WLAN MAC address: \x1b[33;1m%s\n\n", SwitchIdent_GetWirelessLanMacAddress());


	printf("\x1b[22;0H");
	printf("\x1b[36;1m*\x1b[0m Battery lot number: \x1b[36;1m%s          \n\n", SwitchIdent_GetBatteryLot());

	/*
		Misc info:
	*/
	char hostname[128];
	Result ret = gethostname(hostname, sizeof(hostname));

	printf("\x1b[24;0H");
	printf("\x1b[35;1m*\x1b[0m IP: \x1b[35;1m%s\n", R_SUCCEEDED(ret)? hostname : NULL);
	printf("\x1b[35;1m*\x1b[0m Wireless LAN: \x1b[35;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_WirelessLanEnable)? "Enabled" : "Disabled");
	printf("\x1b[35;1m*\x1b[0m Bluetooth: \x1b[35;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_BluetoothEnable)? "Enabled" : "Disabled");
	printf("\x1b[35;1m*\x1b[0m NFC: \x1b[35;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_NfcEnable)? "Enabled" : "Disabled");
	printf("\x1b[35;1m*\x1b[0m Automatic update: \x1b[35;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_AutoUpdateEnable)? "Enabled" : "Disabled");
	printf("\x1b[35;1m*\x1b[0m Console information upload: \x1b[35;1m%s\n", SwitchIdent_GetFlag(SetSysFlag_ConsoleInformationUpload)? "Enabled" : "Disabled");

	printf("\x1b[34;0H");
	printf("\x1b[32;1m> Press the plus key to exit =)\x1b[0m");

	while(appletMainLoop()) {
		/*
			System info (continued):
		*/
		printf("\x1b[10;0H");
		printf("\x1b[33;1m*\x1b[0m CPU clock: \x1b[33;1m%d\x1b[0m MHz          \n", SwitchIdent_GetCPUClock());
		printf("\x1b[33;1m*\x1b[0m GPU clock: \x1b[33;1m%d\x1b[0m MHz          \n", SwitchIdent_GetGPUClock());
		printf("\x1b[33;1m*\x1b[0m EMC clock: \x1b[33;1m%d\x1b[0m MHz          \n", SwitchIdent_GetEMCClock());
		printf("\x1b[33;1m*\x1b[0m Wireless LAN: \x1b[33;1m%s\x1b[0m (RSSI: \x1b[33;1m%d\x1b[0m) (Quality: \x1b[33;1m%d\x1b[0m)          \n", SwitchIdent_GetFlag(SetSysFlag_WirelessLanEnable)? "Enabled" : "Disabled", SwitchIdent_GetWlanRSSI(), SwitchIdent_GetWlanQuality(SwitchIdent_GetWlanRSSI()));

		/*
			Battery info:
		*/
		printf("\x1b[17;0H");
		printf("\x1b[36;1m*\x1b[0m Battery percentage:  \x1b[36;1m%d %%\x1b[0m (\x1b[36;1m%s\x1b[0m) \x1b[0m          \n", SwitchIdent_GetBatteryPercent(), SwitchIdent_IsCharging()? "charging" : "not charging");
		printf("\x1b[36;1m*\x1b[0m Battery voltage state: \x1b[36;1m%s          \n", SwitchIdent_GetVoltageState());
		printf("\x1b[36;1m*\x1b[0m Battery charger type: \x1b[36;1m%s          \n", SwitchIdent_GetChargerType());
		printf("\x1b[36;1m*\x1b[0m Battery charging enabled: \x1b[36;1m%s          \n", SwitchIdent_IsChargingEnabled()? "Yes" : "No");
		printf("\x1b[36;1m*\x1b[0m Battery ample power supplied: \x1b[36;1m%s          \n", SwitchIdent_IsEnoughPowerSupplied()? "Yes" : "No");

		printf("\x1b[30;0H");
		printf("\x1b[35;1m*\x1b[0m State: \x1b[35;1m%s          \n", SwitchIdent_GetOperationMode());
		printf("\x1b[35;1m*\x1b[0m SD card status: \x1b[35;1m%s          \n", isSDInserted? "Inserted" : "Not inserted");
		printf("\x1b[35;1m*\x1b[0m Game card status: \x1b[35;1m%s          \n\n", isGameCardInserted? "Inserted" : "Not inserted");

		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) 
			break; // break in order to return to hbmenu

		consoleUpdate(NULL);
	}

	SwitchIdent_TermServices();

	consoleExit(NULL);
	return 0;
}
