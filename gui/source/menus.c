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

	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "System version:",  SwitchIdent_GetVersion());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "Hardware:", SwitchIdent_GetHardwareType());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "Unit:", SwitchIdent_GetUnit());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 200, "Serial:", SwitchIdent_GetSerialNumber());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 250, "DRAM ID:", SwitchIdent_GetDramDesc());
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 300, "Device ID:", deviceID);
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
	char totalSize[16], freeSize[16], usedSize[16];
	Utils_GetSizeString(totalSize, SwitchIdent_GetTotalStorage(FsStorageId_SdCard));
	Utils_GetSizeString(freeSize, SwitchIdent_GetFreeStorage(FsStorageId_SdCard));
	Utils_GetSizeString(usedSize, SwitchIdent_GetUsedStorage(FsStorageId_SdCard));

	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 50, "Total capacity:",  totalSize);
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 100, "Free capacity:", freeSize);
	Menu_DrawItem(450, 250 + ((MENU_Y_DIST - item_height) / 2) + 150, "Used capacity:", usedSize);
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
			case 4:
				if (kDown & KEY_A)
					break;
				break;
		}
		
		SDL_RenderPresent(RENDERER);

		if (kDown & KEY_PLUS)
			break;
	}
}