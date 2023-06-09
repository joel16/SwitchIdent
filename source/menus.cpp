#include <cstdio>
#include <unistd.h>

#include "common.hpp"
#include "gui.hpp"
#include "menus.hpp"

namespace Menus {
    // Globals
    static int g_item_height = 0;
    static bool g_is_sd_inserted = false, g_is_gamecard_inserted = false;
    static HidsysUniquePadId g_unique_pad_ids[2] = {0};
    static PadState g_pad;
    static const int g_item_dist = 67;
    static const int g_start_x = 450;
    static const int g_start_y = 300;

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
        STATE_CHARGER_INFO,
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

    static void DrawItemf(int x, int y, const char *title, const char *text, ...) {
        int title_width = 0;
        GUI::GetTextDimensions(25, title, &title_width, nullptr);
        GUI::DrawText(x, y, 25, title_colour, title);
        
        char buffer[256];
        va_list args;
        va_start(args, text);
        std::vsnprintf(buffer, 256, text, args);
        GUI::DrawText(x + title_width + 20, y, 25, descr_colour, buffer);
        va_end(args);
    }

    void KernelInfo(void) {
        SetSysFirmwareVersion ver = SwitchIdent::GetFirmwareVersion();
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "Firmware version:", 
            "%u.%u.%u-%u%u", ver.major, ver.minor, ver.micro, ver.revision_major, ver.revision_minor);
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 120, "Hardware:", SwitchIdent::GetHardwareType());
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 180, "Unit:", SwitchIdent::GetUnit());
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 240, "Serial:", SwitchIdent::GetSerialNumber().number);
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 300, "DRAM ID:", SwitchIdent::GetDramDesc());
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 360, "Device ID:", "%llu", SwitchIdent::GetDeviceID());
    }

    void SystemInfo(void) {
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "Region:",  SwitchIdent::GetRegion());
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 120, "CPU clock:", "%lu MHz", SwitchIdent::GetClock(PcvModule_CpuBus));
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 180, "GPU clock:", "%lu MHz", SwitchIdent::GetClock(PcvModule_GPU));
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 240, "EMC clock:", "%lu MHz", SwitchIdent::GetClock(PcvModule_EMC));
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 300, "Wireless LAN:", 
            "%s (RSSI: %d) (Quality: %lu)", SwitchIdent::GetWirelessLanEnableFlag()? "Enabled" : "Disabled", SwitchIdent::GetWlanRSSI(), SwitchIdent::GetWlanQuality(SwitchIdent::GetWlanRSSI()));
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 360, "Bluetooth:", SwitchIdent::GetBluetoothEnableFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 420, "NFC:", SwitchIdent::GetNfcEnableFlag()? "Enabled" : "Disabled");
    }

    void BatteryInfo(void) {
        s32 int_temp = SwitchIdent::GetBatteryTemperature(TsLocation_Internal);
        s32 ext_temp = SwitchIdent::GetBatteryTemperature(TsLocation_External);

        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "Percentage:",  "%lu %% (%s)", SwitchIdent::GetBatteryPercentage(), SwitchIdent::IsCharging()? "charging" : "not charging");
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 120, "Age percentage:", "%.2f %%", SwitchIdent::GetBatteryAgePercentage());
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 180, "Voltage state:", SwitchIdent::GetVoltageState());
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 240, "Lot number:", SwitchIdent::GetBatteryLot().lot);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 300, "Internal temperature:", "%d °C (%d °F)", int_temp, ((int_temp * 9/5) + 32));
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 360, "External temperature:", "%d °C (%d °F)", ext_temp, ((ext_temp * 9/5) + 32));
    }

    void ChargerInfo(void) {
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

        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "Current limit:",  "(In: %d mA) (Out: %d mA)", batteryChargeInfoFields.in_curr_limit, batteryChargeInfoFields.out_curr_limit);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 120, "Battery charging current limit:",  "%d mA", batteryChargeInfoFields.charge_curr_limit);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 180, "Battery charging voltage  limit:",  "%d mV", batteryChargeInfoFields.charge_volt_limit);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 240, "Capacity:",  "%d pcm (Age: %d pcm)", batteryChargeInfoFields.capacity, batteryChargeInfoFields.battery_age);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 300, "Voltage average:", "%d mV", batteryChargeInfoFields.voltage_avg);
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 360, "Charger:", chargers[batteryChargeInfoFields.charger]);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 420, "Charger voltage limit:", "%d mV", batteryChargeInfoFields.charger_volt_limit);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 480, "Charger current limit:", "%d mA", batteryChargeInfoFields.charger_curr_limit);
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
        // TODO: account for HidNpadIdType_Other;
        // Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "JC fw:", "%llu", SwitchIdent::GetJoyconFirmwareVersion(g_unique_pad_ids[0]));

        HidPowerInfo info_left = SwitchIdent::GetJoyconPowerInfoL(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1);
        HidPowerInfo info_right = SwitchIdent::GetJoyconPowerInfoR(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1);

        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "Left Joycon battery:", "%lu %% (%s)", (info_left.battery_level * 25), info_left.is_charging? "charging" : "not charging");
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 120, "Right Joycon battery:", "%lu %% (%s)", (info_right.battery_level * 25), info_right.is_charging? "charging" : "not charging");
    }

    void MiscInfo(void) {
        char hostname[128];
        Result ret = gethostname(hostname, sizeof(hostname));

        SetCalBdAddress bd_addr = SwitchIdent::GetBluetoothBdAddress();
        SetCalMacAddress mac_addr = SwitchIdent::GetWirelessLanMacAddress();

        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 60, "IP:",  R_SUCCEEDED(ret)? hostname : nullptr);
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 120, "State:", SwitchIdent::GetOperationMode());
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 180, "Automatic update:", SwitchIdent::GetAutoUpdateEnableFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 240, "Console information upload:", SwitchIdent::GetConsoleInformationUploadFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 300, "SD card status:", g_is_sd_inserted? "Inserted" : "Not inserted");
        Menus::DrawItem(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 360, "Game card status:", g_is_gamecard_inserted? "Inserted" : "Not inserted");
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 420, "BT address:",
            "%02X:%02X:%02X:%02X:%02X:%02X", bd_addr.bd_addr[0], bd_addr.bd_addr[1], bd_addr.bd_addr[2], bd_addr.bd_addr[3], bd_addr.bd_addr[4], bd_addr.bd_addr[5]);
        Menus::DrawItemf(g_start_x, g_start_y + ((g_item_dist - g_item_height) / 2) + 480, "WLAN address:", 
            "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr.addr[0], mac_addr.addr[1], mac_addr.addr[2], mac_addr.addr[3], mac_addr.addr[4], mac_addr.addr[5]);
    }

    void Main(void) {
        int title_height = 0;
        GUI::GetTextDimensions(25, "SwitchIdent", nullptr, &title_height);
        GUI::GetTextDimensions(25, "Item", nullptr, &g_item_height);
        
        int banner_width = 200;
        int selection = STATE_EXIT;
        Result ret = 0;
        
        FsDeviceOperator fsDeviceOperator;
        if (R_FAILED(ret = fsOpenDeviceOperator(&fsDeviceOperator)))
            std::printf("fsOpenDeviceOperator() failed: 0x%x.\n\n", ret);
            
        g_is_sd_inserted = SwitchIdent::IsSDCardInserted(&fsDeviceOperator);
        g_is_gamecard_inserted = SwitchIdent::IsGameCardInserted(&fsDeviceOperator);
        fsDeviceOperatorClose(&fsDeviceOperator);
        
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&g_pad);
        padUpdate(&g_pad);
        
        
        // For SwitchIdent::GetJoyconFirmwareVersion()
        // memset(g_unique_pad_ids, 0, sizeof(g_unique_pad_ids));

        // s32 total_entries = 0;
        // if (R_FAILED(ret = hidsysGetUniquePadsFromNpad(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1, g_unique_pad_ids, 2, &total_entries)))
        //     std::printf("hidsysGetUniquePadsFromNpad(): 0x%x.\n\n", ret);
        
        // if (R_SUCCEEDED(ret))
        //     std::printf("hidsysGetUniquePadsFromNpad: total_entries (%d)\n", total_entries);

        const char *items[] = {
            "Kernel",
            "System",
            "Battery",
            "Charger",
            "Storage",
            "Joycon",
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
            
            if (kDown & HidNpadButton_AnyDown)
                selection++;
            else if (kDown & HidNpadButton_AnyUp)
                selection--;
                
            if (selection > STATE_EXIT) 
                selection = 0;
            if (selection < 0) 
                selection = STATE_EXIT;
                
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

                case STATE_CHARGER_INFO:
                    Menus::ChargerInfo();
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
            
            if ((kDown & HidNpadButton_Plus) || ((kDown & HidNpadButton_A) && (selection == STATE_EXIT)))
                break;
        }
    }
}
