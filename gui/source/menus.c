#include <unistd.h>
#include <switch.h>

#include "kernel.h"
#include "menus.h"
#include "misc.h"
#include "SDL_helper.h"
#include "storage.h"
#include "system.h"
#include "utils.h"

#define MENU_Y_DIST 67

static int item_height = 0;

static Service setsys_service;

static void Menu_DrawItem(int x, int y, char *item_title, char *item_desc)
{
	int title_width = 0;
	TTF_SizeText(Ubuntu_R, item_title, &title_width, NULL);
	SDL_DrawText(Ubuntu_R, x, y, MENU_INFO_TITLE_COLOUR, item_title);
	SDL_DrawText(Ubuntu_R, x + title_width + 20, y, MENU_INFO_DESC_COLOUR, item_desc);
}

static void Menu_Kernel(void)
{
	static char deviceID[18];
	snprintf(deviceID, 18, "%llu", SwitchIdent_GetDeviceID());

	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "Firmware version:",  SwitchIdent_GetFirmwareVersion(&setsys_service));
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "Kernel version:",  SwitchIdent_GetKernelVersion());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "Hardware:", SwitchIdent_GetHardwareType());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 200, "Unit:", SwitchIdent_GetUnit());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 250, "Serial:", SwitchIdent_GetSerialNumber());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 300, "DRAM ID:", SwitchIdent_GetDramDesc());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 350, "Device ID:", deviceID);
}

static void Menu_System(void)
{
	static char cpu_clock[9], gpu_clock[9];
	snprintf(cpu_clock, 9, "%lu MHz", SwitchIdent_GetCPUClock());
	snprintf(gpu_clock, 9, "%lu MHz", SwitchIdent_GetGPUClock());

	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "Region:",  SwitchIdent_GetRegion());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "CPU clock:", cpu_clock);
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "GPU clock:", gpu_clock);
}

static void Menu_Storage(void)
{
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

	SDL_DrawRect(RENDERER, 400, 50, 880, 670, BACKGROUND_COLOUR);

	SDL_DrawImage(RENDERER, drive, 450, 88, 128, 128);
	SDL_DrawRect(RENDERER, 450, 226, 128, 25, STATUS_BAR_COLOUR);
	SDL_DrawRect(RENDERER, 452, 228, 124, 21, BACKGROUND_COLOUR);
	SDL_DrawRect(RENDERER, 452, 228, (((double)sd_used / (double)sd_total) * 124.0), 21, MENU_SELECTOR_COLOUR);

	SDL_DrawImage(RENDERER, drive, 450, 296, 128, 128);
	SDL_DrawRect(RENDERER, 450, 434, 128, 25, STATUS_BAR_COLOUR);
	SDL_DrawRect(RENDERER, 452, 436, 124, 21, BACKGROUND_COLOUR);
	SDL_DrawRect(RENDERER, 452, 436, (((double)nand_u_used / (double)nand_u_total) * 124.0), 21, MENU_SELECTOR_COLOUR);
	
	SDL_DrawImage(RENDERER, drive, 450, 504, 128, 128);
	SDL_DrawRect(RENDERER, 450, 642, 128, 25, STATUS_BAR_COLOUR);
	SDL_DrawRect(RENDERER, 452, 644, 124, 21, BACKGROUND_COLOUR);
	SDL_DrawRect(RENDERER, 452, 644, (((double)nand_s_used / (double)nand_s_total) * 124.0), 21, MENU_SELECTOR_COLOUR);

	SDL_DrawText(Ubuntu_R, 600, 38 + ((MENU_Y_DIST - item_height) / 2) + 50, MENU_INFO_DESC_COLOUR, "SD");
	Menu_DrawItem(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 88, "Total storage capacity:",  sd_total_str);
	Menu_DrawItem(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 126, "Free storage capacity:", sd_free_str);
	Menu_DrawItem(600, 38 + ((MENU_Y_DIST - item_height) / 2) + 164, "Used storage capacity:", sd_used_str);

	SDL_DrawText(Ubuntu_R, 600, 246 + ((MENU_Y_DIST - item_height) / 2) + 50, MENU_INFO_DESC_COLOUR, "NAND User");
	Menu_DrawItem(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 88, "Total storage capacity:",  nand_u_total_str);
	Menu_DrawItem(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 126, "Free storage capacity:", nand_u_free_str);
	Menu_DrawItem(600, 246 + ((MENU_Y_DIST - item_height) / 2) + 164, "Used storage capacity:", nand_u_used_str);

	SDL_DrawText(Ubuntu_R, 600, 454 + ((MENU_Y_DIST - item_height) / 2) + 50, MENU_INFO_DESC_COLOUR, "NAND System");
	Menu_DrawItem(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 88, "Total storage capacity:",  nand_s_total_str);
	Menu_DrawItem(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 126, "Free storage capacity:", nand_s_free_str);
	Menu_DrawItem(600, 454 + ((MENU_Y_DIST - item_height) / 2) + 164, "Used storage capacity:", nand_s_used_str);
}

static void Menu_Misc(void)
{
	char hostname[128];
	Result ret = gethostname(hostname, sizeof(hostname));
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "IP:",  R_SUCCEEDED(ret)? hostname : NULL);
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "State:", SwitchIdent_GetOperationMode());
}

void Menu_Main(void)
{
	int title_height = 0;
	TTF_SizeText(Ubuntu_R, "SwitchIdent", NULL, &title_height);

	TTF_SizeText(Ubuntu_R, "Test", NULL, &item_height);

	int banner_width = 0, banner_height = 0;
	SDL_QueryTexture(banner, NULL, NULL, &banner_width, &banner_height);

	int selection = 0;

	Result ret = 0;
	if (R_FAILED(ret = smGetService(&setsys_service, "set:sys")))
		printf("setsysInitialize() failed: 0x%x.\n\n", ret);

	while(appletMainLoop())
	{
		SDL_ClearScreen(RENDERER, BACKGROUND_COLOUR);
		SDL_RenderClear(RENDERER);
		SDL_DrawRect(RENDERER, 0, 0, 1280, 50, STATUS_BAR_COLOUR);
		SDL_DrawRect(RENDERER, 0, 50, 400, 670, MENU_BAR_COLOUR);

		SDL_DrawText(Ubuntu_R, 40, ((50 - title_height) / 2), BACKGROUND_COLOUR, "SwitchIdent");

		SDL_DrawImage(RENDERER, banner, 400 + ((880 - (banner_width)) / 2),  80, banner_width, banner_height);

		SDL_DrawRect(RENDERER, 0, 50 + (MENU_Y_DIST * selection), 400, MENU_Y_DIST, MENU_SELECTOR_COLOUR);

		SDL_DrawText(Ubuntu_R, 30, 50 + ((MENU_Y_DIST - item_height) / 2), selection == 0? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Kernel");
		SDL_DrawText(Ubuntu_R, 30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 1), selection == 1? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "System");
		SDL_DrawText(Ubuntu_R, 30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 2), selection == 2? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Storage");
		SDL_DrawText(Ubuntu_R, 30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 3), selection == 3? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Misc");
		SDL_DrawText(Ubuntu_R, 30, 50 + ((MENU_Y_DIST - item_height) / 2) + (MENU_Y_DIST * 4), selection == 4? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Exit");

		hidScanInput();
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_DDOWN)
			selection++;
		else if (kDown & KEY_DUP)
			selection--;

		if (selection > 4) 
			selection = 0;
		if (selection < 0) 
			selection = 4;

		switch (selection)
		{
			case 0:
				Menu_Kernel();
				break;
			case 1:
				Menu_System();
				break;
			case 2:
				Menu_Storage();
				break;
			case 3:
				Menu_Misc();
				break;
		}
		
		SDL_RenderPresent(RENDERER);

		if ((kDown & KEY_PLUS) || ((kDown & KEY_A) && (selection == 4)))
			break;
	}

	serviceClose(&setsys_service);
}