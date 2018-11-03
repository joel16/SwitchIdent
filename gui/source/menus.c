#include <unistd.h>
#include <switch.h>

#include "fs.h"
#include "kernel.h"
#include "menus.h"
#include "misc.h"
#include "power.h"
#include "SDL_helper.h"
#include "storage.h"
#include "system.h"
#include "utils.h"
#include "wlan.h"

#define MENU_Y_DIST    67
#define MAX_MENU_ITEMS 5

static u32 item_height = 0;

static Service setcal_service, psm_service, wlaninf_service;
//static FsDeviceOperator fsDeviceOperator;

static void Menu_DrawItem(int x, int y, char *item_title, const char *text, ...) {
	u32 title_width = 0;
	SDL_GetTextDimensions(25, item_title, &title_width, NULL);
	SDL_DrawText(x, y, 25, MENU_INFO_TITLE_COLOUR, item_title);
	
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	SDL_DrawText(x + title_width + 20, y, 25, MENU_INFO_DESC_COLOUR, buffer);
	va_end(args);
}

static void Menu_Kernel(void) {
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "Firmware version:",  SwitchIdent_GetFirmwareVersion());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "Kernel version:",  SwitchIdent_GetKernelVersion());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "Hardware:", SwitchIdent_GetHardwareType());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 200, "Unit:", SwitchIdent_GetUnit());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 250, "Serial:", SwitchIdent_GetSerialNumber());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 300, "DRAM ID:", SwitchIdent_GetDramDesc());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 350, "Device ID:", "%llu", SwitchIdent_GetDeviceID());
}

static void Menu_System(void) {
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "Region:",  SwitchIdent_GetRegion());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "CPU clock:", "%lu MHz", SwitchIdent_GetCPUClock());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "GPU clock:", "%lu MHz", SwitchIdent_GetGPUClock());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 200, "Wireless LAN:", "%s (RSSI: %d) (Quality: %lu)", SwitchIdent_GetFlag(SetSysFlag_WirelessLanEnable)? "Enabled" : "Disabled", SwitchIdent_GetWlanRSSI(&wlaninf_service), SwitchIdent_GetWlanQuality(SwitchIdent_GetWlanRSSI(&wlaninf_service)));
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 250, "Bluetooth:", "%s", SwitchIdent_GetFlag(SetSysFlag_BluetoothEnable)? "Enabled" : "Disabled");
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 300, "NFC:", SwitchIdent_GetFlag(SetSysFlag_NfcEnable)? "Enabled" : "Disabled");
}

static void Menu_Power(void) {
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "Battery percentage:",  "%lu %% (%s)", SwitchIdent_GetBatteryPercent(), SwitchIdent_IsCharging()? "charging" : "not charging");
	//Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "Battery voltage state:", SwitchIdent_GetVoltageState(&psm_service));
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "Battery charger type:", SwitchIdent_GetChargerType());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "Battery charging enabled:", SwitchIdent_IsChargingEnabled(&psm_service)? "Yes" : "No");
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 200, "Battery ample power supplied:", SwitchIdent_IsEnoughPowerSupplied(&psm_service)? "Yes" : "No");
}

static void Menu_Storage(void) {
	u64 sd_used = SwitchIdent_GetUsedStorage(FsStorageId_SdCard);
	u64 sd_total = SwitchIdent_GetTotalStorage(FsStorageId_SdCard);

	u64 nand_u_used = SwitchIdent_GetUsedStorage(FsStorageId_NandUser);
	u64 nand_u_total = SwitchIdent_GetTotalStorage(FsStorageId_NandUser);

	u64 nand_s_used = SwitchIdent_GetUsedStorage(FsStorageId_NandSystem);
	u64 nand_s_total = SwitchIdent_GetTotalStorage(FsStorageId_NandSystem);

	char sd_total_str[16], sd_free_str[16], sd_used_str[16];
	Utils_GetSizeString(sd_total_str, sd_total);
	Utils_GetSizeString(sd_free_str, SwitchIdent_GetFreeStorage(FsStorageId_SdCard));
	Utils_GetSizeString(sd_used_str, sd_used);

	char nand_u_total_str[16], nand_u_free_str[16], nand_u_used_str[16];
	Utils_GetSizeString(nand_u_total_str, nand_u_total);
	Utils_GetSizeString(nand_u_free_str, SwitchIdent_GetFreeStorage(FsStorageId_NandUser));
	Utils_GetSizeString(nand_u_used_str, nand_u_used);

	char nand_s_total_str[16], nand_s_free_str[16], nand_s_used_str[16];
	Utils_GetSizeString(nand_s_total_str, nand_s_total);
	Utils_GetSizeString(nand_s_free_str, SwitchIdent_GetFreeStorage(FsStorageId_NandSystem));
	Utils_GetSizeString(nand_s_used_str, nand_s_used);

	SDL_DrawRect(400, 50, 880, 670, BACKGROUND_COLOUR);

	SDL_DrawDriveIcon(450, 88);
	SDL_DrawRect(450, 226, 128, 25, STATUS_BAR_COLOUR);
	SDL_DrawRect(452, 228, 124, 21, BACKGROUND_COLOUR);
	SDL_DrawRect(452, 228, (((double)sd_used / (double)sd_total) * 124.0), 21, MENU_SELECTOR_COLOUR);

	SDL_DrawDriveIcon(450, 296);
	SDL_DrawRect(450, 434, 128, 25, STATUS_BAR_COLOUR);
	SDL_DrawRect(452, 436, 124, 21, BACKGROUND_COLOUR);
	SDL_DrawRect(452, 436, (((double)nand_u_used / (double)nand_u_total) * 124.0), 21, MENU_SELECTOR_COLOUR);

	SDL_DrawDriveIcon(450, 504);
	SDL_DrawRect(450, 642, 128, 25, STATUS_BAR_COLOUR);
	SDL_DrawRect(452, 644, 124, 21, BACKGROUND_COLOUR);
	SDL_DrawRect(452, 644, (((double)nand_s_used / (double)nand_s_total) * 124.0), 21, MENU_SELECTOR_COLOUR);

	SDL_DrawText(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 50, 25, MENU_INFO_DESC_COLOUR, "SD");
	Menu_DrawItem(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 88, "Total storage capacity:",  sd_total_str);
	Menu_DrawItem(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 126, "Free storage capacity:", sd_free_str);
	Menu_DrawItem(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 164, "Used storage capacity:", sd_used_str);

	SDL_DrawText(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 50, 25, MENU_INFO_DESC_COLOUR, "NAND User");
	Menu_DrawItem(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 88, "Total storage capacity:",  nand_u_total_str);
	Menu_DrawItem(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 126, "Free storage capacity:", nand_u_free_str);
	Menu_DrawItem(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 164, "Used storage capacity:", nand_u_used_str);

	SDL_DrawText(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 50, 25, MENU_INFO_DESC_COLOUR, "NAND System");
	Menu_DrawItem(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 88, "Total storage capacity:",  nand_s_total_str);
	Menu_DrawItem(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 126, "Free storage capacity:", nand_s_free_str);
	Menu_DrawItem(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 164, "Used storage capacity:", nand_s_used_str);
}

static void Menu_Misc(void) {
	char hostname[128];
	Result ret = gethostname(hostname, sizeof(hostname));
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "IP:",  R_SUCCEEDED(ret)? hostname : NULL);
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "State:", SwitchIdent_GetOperationMode());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "Automatic update:", SwitchIdent_GetFlag(SetSysFlag_AutoUpdateEnable)? "Enabled" : "Disabled");
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 200, "Console information upload:", SwitchIdent_GetFlag(SetSysFlag_ConsoleInformationUpload)? "Enabled" : "Disabled");
	//Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 250, "SD card status:", SwitchIdent_IsSDCardInserted(&fsDeviceOperator)? "Inserted" : "Not inserted");
	//Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 300, "Game card status:", SwitchIdent_IsGameCardInserted(&fsDeviceOperator)? "Inserted" : "Not inserted");
}

void Menu_Main(void) {
	u32 title_height = 0;
	SDL_GetTextDimensions(25, "SwitchIdent", NULL, &title_height);
	SDL_GetTextDimensions(25, "Test", NULL, &item_height);

	int banner_width = 200;
	int selection = 0;
	Result ret = 0;

	if (R_FAILED(ret = smGetService(&setcal_service, "set:cal")))
		printf("setcalInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = smGetService(&psm_service, "psm")))
		printf("psmInitialize() failed: 0x%x.\n\n", ret);

	if (R_FAILED(ret = smGetService(&wlaninf_service, "wlan:inf")))
		printf("wlaninfInitialize() failed: 0x%x.\n\n", ret);

	/*if (R_FAILED(ret = fsOpenDeviceOperator(&fsDeviceOperator)))
		printf("fsOpenDeviceOperator() failed: 0x%x.\n\n", ret);*/

	while(appletMainLoop()) {
		SDL_ClearScreen(BACKGROUND_COLOUR);
		SDL_DrawRect(0, 0, 1280, 50, STATUS_BAR_COLOUR);
		SDL_DrawRect(0, 50, 400, 670, MENU_BAR_COLOUR);

		SDL_DrawTextf(30, ((50 - title_height) / 2), 25, BACKGROUND_COLOUR, "SwitchIdent v%d.%d", VERSION_MAJOR, VERSION_MINOR);// 0x%lx 0x%lx", connect, disconnect);

		SDL_DrawBanner(400 + ((880 - (banner_width)) / 2),  80);

		SDL_DrawRect(0, 50 + (MENU_Y_DIST * selection), 400, MENU_Y_DIST, MENU_SELECTOR_COLOUR);

		SDL_DrawText(30, 50 + ((MENU_Y_DIST - item_height) / 2), 25, selection == 0? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "\ue140  Kernel");
		SDL_DrawText(30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 1), 25, selection == 1? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "\ue142  System");
		SDL_DrawText(30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 2), 25, selection == 2? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "\ue0b7  Power");
		SDL_DrawText(30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 3), 25, selection == 3? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "\ue135  Storage");
		SDL_DrawText(30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 4), 25, selection == 4? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "\ue130  Misc");
		SDL_DrawText(30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 5), 25, selection == 5? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "\ue145  Exit");

		hidScanInput();
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_DDOWN)
			selection++;
		else if (kDown & KEY_DUP)
			selection--;

		if (selection > MAX_MENU_ITEMS) 
			selection = 0;
		if (selection < 0) 
			selection = MAX_MENU_ITEMS;

		switch (selection) {
			case 0:
				Menu_Kernel();
				break;
			case 1:
				Menu_System();
				break;
			case 2:
				Menu_Power();
				break;
			case 3:
				Menu_Storage();
				break;
			case 4:
				Menu_Misc();
				break;
		}
		
		SDL_Renderdisplay();

		if ((kDown & KEY_PLUS) || ((kDown & KEY_A) && (selection == MAX_MENU_ITEMS)))
			break;
	}

	//fsDeviceOperatorClose(&fsDeviceOperator);
	serviceClose(&wlaninf_service);
	serviceClose(&psm_service);
	serviceClose(&setcal_service);
}
