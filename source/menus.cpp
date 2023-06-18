#include <cstdio>
#include <unistd.h>

#include "common.hpp"
#include "gui.hpp"
#include "menus.hpp"

namespace Menus {
    // Globals
    static int g_item_height = 0;
    static bool g_is_sd_inserted = false, g_is_gamecard_inserted = false;
    static PadState g_pad;
    static const int g_item_dist = 67;
    static const int g_start_x = 450;
    static const int g_start_y = 260;

    // Colours
    static const SDL_Color bg_colour = { 62, 62, 62 };
    static const SDL_Color status_bar_colour = { 44, 44, 44 };
    static const SDL_Color menu_bar_colour = { 52, 52, 52 };
    static const SDL_Color selector_colour = { 223, 74, 22 };
    static const SDL_Color title_colour = { 252, 252, 252 };
    static const SDL_Color descr_colour = { 182, 182, 182 };
    
    enum MenuState {
        STATE_KERNEL_INFO = 0,
        STATE_SYSTEM_INFO,
        STATE_BATTERY_INFO,
        STATE_STORAGE_INFO,
        STATE_JOYCON_INFO,
        STATE_MISC_INFO,
        STATE_EXIT,
        MAX_ITEMS
    };
    
    static void DrawItem(int x, int y, const char *title, const char *text) {
        int title_width = 0;
        GUI::GetTextDimensions(25, title, &title_width, nullptr);
        GUI::DrawText(x, y, 25, title_colour, title);
        GUI::DrawText(x + title_width + 20, y, 25, descr_colour, text);
    }

    static void DrawItem(int index, const char *title, const char *text) {
        int title_width = 0;
        int y = g_start_y + ((g_item_dist - g_item_height) / 2) + 60 * index;
        GUI::GetTextDimensions(25, title, &title_width, nullptr);
        GUI::DrawText(g_start_x, y, 25, title_colour, title);
        GUI::DrawText(g_start_x + title_width + 20, y, 25, descr_colour, text);
    }

    static void DrawItemf(int index, const char *title, const char *text, ...) {
        int title_width = 0;
        int y = g_start_y + ((g_item_dist - g_item_height) / 2) + 60 * index;
        GUI::GetTextDimensions(25, title, &title_width, nullptr);
        GUI::DrawText(g_start_x, y, 25, title_colour, title);
        
        char buffer[256];
        va_list args;
        va_start(args, text);
        std::vsnprintf(buffer, 256, text, args);
        GUI::DrawText(g_start_x + title_width + 20, y, 25, descr_colour, buffer);
        va_end(args);
    }

    void KernelInfo(void) {
        SetSysFirmwareVersion ver = SwitchIdent::GetFirmwareVersion();
        //DockFirmwareVersion dock_ver = SwitchIdent::GetDockFirmware();
        Menus::DrawItemf(1, "Firmware version:", "%u.%u.%u-%u%u", ver.major, ver.minor, ver.micro, ver.revision_major, ver.revision_minor);
        Menus::DrawItem(2, "Hardware:", SwitchIdent::GetHardwareType());
        Menus::DrawItem(3, "Unit:", SwitchIdent::GetUnit());
        Menus::DrawItem(4, "Serial:", SwitchIdent::GetSerialNumber().number);
        Menus::DrawItem(5, "DRAM ID:", SwitchIdent::GetDramDesc());
        Menus::DrawItemf(6, "Device ID:", "%llu", SwitchIdent::GetDeviceID());

        // if (hosversionAtLeast(2, 0, 0) && appletGetOperationMode() == AppletOperationMode_Console) {
        //     Menus::DrawItemf(7, "Dock firmware:", "%d.%d.%d.%d", dock_ver.major, dock_ver.minor, dock_ver.micro, dock_ver.rev);
        // }
    }

    void SystemInfo(void) {
        s32 int_temp = SwitchIdent::GetBatteryTemperature(TsLocation_Internal);
        s32 ext_temp = SwitchIdent::GetBatteryTemperature(TsLocation_External);

        Menus::DrawItem(1, "Region:",  SwitchIdent::GetRegion());
        Menus::DrawItemf(2, "CPU clock:", "%lu MHz", SwitchIdent::GetClock(PcvModule_CpuBus));
        Menus::DrawItemf(3, "GPU clock:", "%lu MHz", SwitchIdent::GetClock(PcvModule_GPU));
        Menus::DrawItemf(4, "EMC clock:", "%lu MHz", SwitchIdent::GetClock(PcvModule_EMC));
        Menus::DrawItemf(5, "Wireless LAN:", "%s (RSSI: %d) (Quality: %lu)", SwitchIdent::GetWirelessLanEnableFlag()? "Enabled" : "Disabled", SwitchIdent::GetWlanRSSI(), SwitchIdent::GetWlanQuality(SwitchIdent::GetWlanRSSI()));
        Menus::DrawItem(6, "Bluetooth:", SwitchIdent::GetBluetoothEnableFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(7, "NFC:", SwitchIdent::GetNfcEnableFlag()? "Enabled" : "Disabled");
        Menus::DrawItemf(8, "Internal (PCB) temperature:", "%d °C (%d °F)", int_temp, ((int_temp * 9/5) + 32));
        Menus::DrawItemf(9, "External (SoC) temperature:", "%d °C (%d °F)", ext_temp, ((ext_temp * 9/5) + 32));
        Menus::DrawItem(10, "Headphone Status:",  SwitchIdent::GetHeadphoneStatus());
    }

    void BatteryInfo(void) {
        const char *chargers[] = {
            "Unknown",
            "PD",
            "Type-C 1.5A",
            "Type-C 3.0A"
            "DCP",
            "CDP",
            "SDP",
            "Apple 500mA",
            "Apple 1000mA",
            "Apple 2000mA"
        };

        BatteryChargeInfoFields batteryChargeInfoFields = { 0 };
        SwitchIdent::GetBatteryChargeInfoFields(&batteryChargeInfoFields);

        u32 battery_temp = batteryChargeInfoFields.battery_temp * 0.001;

        Menus::DrawItemf(1, "Battery Percentage:",  "%lu %% (%s)", SwitchIdent::GetBatteryPercentage(), SwitchIdent::IsCharging()? "charging" : "not charging");
        Menus::DrawItemf(2, "Battery age percentage:", "%.2f %%", SwitchIdent::GetBatteryAgePercentage());
        Menus::DrawItemf(3, "Battery temperature", "%d °C (%d °F)", battery_temp, ((battery_temp * 9/5) + 32));
        Menus::DrawItemf(4, "Battery voltage state:", "%s (Avg: %d mV)", SwitchIdent::GetVoltageState(), batteryChargeInfoFields.voltage_avg);
        Menus::DrawItem(5, "Battery lot number:", SwitchIdent::GetBatteryLot().lot);
        Menus::DrawItemf(6, "Charger current limit:",  "(In: %d mA) (Out: %d mA)", batteryChargeInfoFields.in_curr_limit, batteryChargeInfoFields.out_curr_limit);
        Menus::DrawItemf(7, "Battery charging current limit:",  "%d mA", batteryChargeInfoFields.charge_curr_limit);
        Menus::DrawItemf(8, "Battery charging voltage  limit:",  "%d mV", batteryChargeInfoFields.charge_volt_limit);
        Menus::DrawItemf(9, "Battery capacity:",  "%d pcm (Age: %d pcm)", batteryChargeInfoFields.capacity, batteryChargeInfoFields.battery_age);
        Menus::DrawItem(10, "Charger:", chargers[batteryChargeInfoFields.charger]);
        Menus::DrawItemf(11, "Charger voltage limit:", "%d mV", batteryChargeInfoFields.charger_volt_limit);
        Menus::DrawItemf(12, "Charger current limit:", "%d mA", batteryChargeInfoFields.charger_curr_limit);
    }

    void StorageInfo(void) {
        u64 sd_used = SwitchIdent::GetUsedStorage(NcmStorageId_SdCard);
        u64 sd_total = SwitchIdent::GetTotalStorage(NcmStorageId_SdCard);
        
        u64 nand_u_used = SwitchIdent::GetUsedStorage(NcmStorageId_BuiltInUser);
        u64 nand_u_total = SwitchIdent::GetTotalStorage(NcmStorageId_BuiltInUser);
        
        u64 nand_s_used = SwitchIdent::GetUsedStorage(NcmStorageId_BuiltInSystem);
        u64 nand_s_total = SwitchIdent::GetTotalStorage(NcmStorageId_BuiltInSystem);
        
        char sd_total_str[16], sd_free_str[16], sd_used_str[16];
        SwitchIdent::GetSizeString(sd_total_str, sd_total);
        SwitchIdent::GetSizeString(sd_free_str, SwitchIdent::GetFreeStorage(NcmStorageId_SdCard));
        SwitchIdent::GetSizeString(sd_used_str, sd_used);
        
        char nand_u_total_str[16], nand_u_free_str[16], nand_u_used_str[16];
        SwitchIdent::GetSizeString(nand_u_total_str, nand_u_total);
        SwitchIdent::GetSizeString(nand_u_free_str, SwitchIdent::GetFreeStorage(NcmStorageId_BuiltInUser));
        SwitchIdent::GetSizeString(nand_u_used_str, nand_u_used);
        
        char nand_s_total_str[16], nand_s_free_str[16], nand_s_used_str[16];
        SwitchIdent::GetSizeString(nand_s_total_str, nand_s_total);
        SwitchIdent::GetSizeString(nand_s_free_str, SwitchIdent::GetFreeStorage(NcmStorageId_BuiltInSystem));
        SwitchIdent::GetSizeString(nand_s_used_str, nand_s_used);
        
        GUI::DrawRect(400, 50, 880, 670, bg_colour);
        
        GUI::DrawImage(drive, 450, 88);
        GUI::DrawRect(450, 226, 128, 25, descr_colour);
        GUI::DrawRect(452, 228, 124, 21, bg_colour);
        GUI::DrawRect(452, 228, (((double)sd_used / (double)sd_total) * 124.0), 21, selector_colour);
        
        GUI::DrawImage(drive, 450, 296);
        GUI::DrawRect(450, 434, 128, 25, descr_colour);
        GUI::DrawRect(452, 436, 124, 21, bg_colour);
        GUI::DrawRect(452, 436, (((double)nand_u_used / (double)nand_u_total) * 124.0), 21, selector_colour);
        
        GUI::DrawImage(drive, 450, 504);
        GUI::DrawRect(450, 642, 128, 25, descr_colour);
        GUI::DrawRect(452, 644, 124, 21, bg_colour);
        GUI::DrawRect(452, 644, (((double)nand_s_used / (double)nand_s_total) * 124.0), 21, selector_colour);
        
        GUI::DrawText(600, 38 + ((g_item_dist - g_item_height) / 2) + 50, 25, descr_colour, "SD");
        Menus::DrawItem(600, 38 + ((g_item_dist - g_item_height) / 2) + 88, "Total storage capacity:",  sd_total_str);
        Menus::DrawItem(600, 38 + ((g_item_dist - g_item_height) / 2) + 126, "Free storage capacity:", sd_free_str);
        Menus::DrawItem(600, 38 + ((g_item_dist - g_item_height) / 2) + 164, "Used storage capacity:", sd_used_str);
        
        GUI::DrawText(600, 246 + ((g_item_dist - g_item_height) / 2) + 50, 25, descr_colour, "NAND User");
        Menus::DrawItem(600, 246 + ((g_item_dist - g_item_height) / 2) + 88, "Total storage capacity:",  nand_u_total_str);
        Menus::DrawItem(600, 246 + ((g_item_dist - g_item_height) / 2) + 126, "Free storage capacity:", nand_u_free_str);
        Menus::DrawItem(600, 246 + ((g_item_dist - g_item_height) / 2) + 164, "Used storage capacity:", nand_u_used_str);
        
        GUI::DrawText(600, 454 + ((g_item_dist - g_item_height) / 2) + 50, 25, descr_colour, "NAND System");
        Menus::DrawItem(600, 454 + ((g_item_dist - g_item_height) / 2) + 88, "Total storage capacity:",  nand_s_total_str);
        Menus::DrawItem(600, 454 + ((g_item_dist - g_item_height) / 2) + 126, "Free storage capacity:", nand_s_free_str);
        Menus::DrawItem(600, 454 + ((g_item_dist - g_item_height) / 2) + 164, "Used storage capacity:", nand_s_used_str);
    }

    void JoyconInfo(void) {
        // TODO: Get info on other connected controllers
        HidPowerInfo info_left = SwitchIdent::GetJoyconPowerInfoL(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1);
        HidPowerInfo info_right = SwitchIdent::GetJoyconPowerInfoR(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1);

        HIDFirmwareVersion version_left;
        HIDFirmwareVersion version_right;
        SwitchIdent::GetJoyconFirmwareVersion(padIsHandheld(&g_pad) ? HidDeviceTypeBits_HandheldLeft : HidDeviceTypeBits_JoyLeft, &version_left);
        SwitchIdent::GetJoyconFirmwareVersion(padIsHandheld(&g_pad) ? HidDeviceTypeBits_HandheldRight : HidDeviceTypeBits_JoyRight, &version_right);

        Menus::DrawItemf(1, "Left Joy-con battery:", "%lu %% (%s)", (info_left.battery_level * 25), info_left.is_charging? "charging" : "not charging");
        Menus::DrawItemf(2, "Right Joy-con battery:", "%lu %% (%s)", (info_right.battery_level * 25), info_right.is_charging? "charging" : "not charging");
        Menus::DrawItemf(3, "Left Joy-con firmware:", "%d.%d.%d", version_left.major, version_left.minor, version_left.micro);
        Menus::DrawItemf(4, "Right Joy-con firmware:", "%d.%d.%d", version_right.major, version_right.minor, version_right.micro);
    }

    void MiscInfo(void) {
        char hostname[128];
        Result ret = gethostname(hostname, sizeof(hostname));

        SetCalBdAddress bd_addr = SwitchIdent::GetBluetoothBdAddress();
        SetCalMacAddress mac_addr = SwitchIdent::GetWirelessLanMacAddress();

        Menus::DrawItem(1, "IP:",  R_SUCCEEDED(ret)? hostname : nullptr);
        Menus::DrawItem(2, "State:", SwitchIdent::GetOperationMode());
        Menus::DrawItem(3, "Automatic update:", SwitchIdent::GetAutoUpdateEnableFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(4, "Console information upload:", SwitchIdent::GetConsoleInformationUploadFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(5, "SD card status:", g_is_sd_inserted? "Inserted" : "Not inserted");
        Menus::DrawItem(6, "Game card status:", g_is_gamecard_inserted? "Inserted" : "Not inserted");
        Menus::DrawItemf(7, "BT address:", "%02X:%02X:%02X:%02X:%02X:%02X", bd_addr.bd_addr[0], bd_addr.bd_addr[1], bd_addr.bd_addr[2], bd_addr.bd_addr[3], bd_addr.bd_addr[4], bd_addr.bd_addr[5]);
        Menus::DrawItemf(8, "WLAN address:", "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr.addr[0], mac_addr.addr[1], mac_addr.addr[2], mac_addr.addr[3], mac_addr.addr[4], mac_addr.addr[5]);
    }

    void Main(void) {
        int title_height = 0;
        GUI::GetTextDimensions(25, "SwitchIdent", nullptr, &title_height);
        GUI::GetTextDimensions(25, "Item", nullptr, &g_item_height);
        
        int banner_width = 200;
        int selection = STATE_KERNEL_INFO;
        Result ret = 0;
        
        FsDeviceOperator fsDeviceOperator;
        if (R_FAILED(ret = fsOpenDeviceOperator(&fsDeviceOperator))) {
            std::printf("fsOpenDeviceOperator() failed: 0x%x.\n\n", ret);
        }
            
        g_is_sd_inserted = SwitchIdent::IsSDCardInserted(&fsDeviceOperator);
        g_is_gamecard_inserted = SwitchIdent::IsGameCardInserted(&fsDeviceOperator);
        fsDeviceOperatorClose(&fsDeviceOperator);
        
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&g_pad);
        padUpdate(&g_pad);

        const char *items[] = {
            "Kernel",
            "System",
            "Battery",
            "Storage",
            "Joy-con",
            "Misc",
            "Exit"
        };

        while(appletMainLoop()) {
            GUI::ClearScreen(bg_colour);
            GUI::DrawRect(0, 0, 1920, 50, status_bar_colour);
            GUI::DrawRect(0, 50, 400, 1030, menu_bar_colour);
            
            GUI::DrawTextf(30, ((50 - title_height) / 2), 25, title_colour, "SwitchIdent Test v%d.%d", VERSION_MAJOR, VERSION_MINOR);
            GUI::DrawImage(banner, 400 + ((1320 - (banner_width)) / 2),  80);
            
            GUI::DrawRect(0, 50 + (g_item_dist * selection), 400, g_item_dist, selector_colour);

            for (int i = 0; i < MAX_ITEMS; i++) {
                GUI::DrawImage(menu_icons[i], 20, 52 + ((g_item_dist - g_item_height) / 2) + (g_item_dist * i));
                GUI::DrawText(75, 45 + ((g_item_dist - g_item_height) / 2) + (g_item_dist * i), 25, title_colour, items[i]);
            }
            
            padUpdate(&g_pad);
            u32 kDown = padGetButtonsDown(&g_pad);
            
            if (kDown & HidNpadButton_AnyDown) {
                selection++;
            }
            else if (kDown & HidNpadButton_AnyUp) {
                selection--;
            }
                
            if (selection > STATE_EXIT) {
                selection = 0;
            }
            if (selection < 0) {
                selection = STATE_EXIT;
            }

            switch (selection) {
                case STATE_KERNEL_INFO:
                    Menus::KernelInfo();
                    break;
                
                case STATE_SYSTEM_INFO:
                    Menus::SystemInfo();
                    break;
                
                case STATE_BATTERY_INFO:
                    Menus::BatteryInfo();
                    break;
                
                case STATE_STORAGE_INFO:
                    Menus::StorageInfo();
                    break;

                case STATE_JOYCON_INFO:
                    Menus::JoyconInfo();
                    break;
                    
                case STATE_MISC_INFO:
                    Menus::MiscInfo();
                    break;

                default:
                    break;
            }
            
            GUI::Render();
            
            if ((kDown & HidNpadButton_Plus) || ((kDown & HidNpadButton_A) && (selection == STATE_EXIT))) {
                break;
            }
        }
    }
}
