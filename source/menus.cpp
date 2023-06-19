#include <cstdio>
#include <unistd.h>

#include "common.hpp"
#include "gui.hpp"
#include "menus.hpp"

namespace Menus {
    // Globals
    static int g_item_height = 0;
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
    
    enum PageState {
        KERNEL_INFO_PAGE = 0,
        SYSTEM_INFO_PAGE,
        BATTERY_INFO_PAGE,
        STORAGE_INFO_PAGE,
        JOYCON_INFO_PAGE,
        MISC_INFO_PAGE,
        EXIT_PAGE,
        MAX_ITEMS
    };

    typedef struct {
        SetSysFirmwareVersion version;
        DockFirmwareVersion dock_version;
        const char *hardware_type;
        const char *unit;
        SetSysSerialNumber serial_number;
        const char *dram_desc;
        u64 device_id;
    } KernelInfo;

    typedef struct {
        const char *region;
    } SystemInfo;

    typedef struct {
        SetBatteryLot battery_lot;
    } BatteryInfo;

    typedef struct {
        u64 sd_used;
        u64 sd_total;
        char sd_used_str[16];
        char sd_free_str[16];
        char sd_total_str[16];
        u64 nand_u_used;
        u64 nand_u_total;
        char nand_u_total_str[16];
        char nand_u_free_str[16];
        char nand_u_used_str[16];
        u64 nand_s_used;
        u64 nand_s_total;
        char nand_s_total_str[16];
        char nand_s_free_str[16];
        char nand_s_used_str[16];
    } StorageInfo;

    typedef struct {
        HIDFirmwareVersion version_left;
        HIDFirmwareVersion version_right;
    } JoyconInfo;

    typedef struct {
        bool sd_inserted;
        bool gamecard_inserted;
        SetCalBdAddress bd_addr;
        SetCalMacAddress mac_addr;
    } MiscInfo;
    
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

    static void KernelInfoPage(const KernelInfo& kernel_info) {
        Menus::DrawItemf(1, "Firmware version:", "%u.%u.%u-%u%u", kernel_info.version.major, kernel_info.version.minor, kernel_info.version.micro, kernel_info.version.revision_major, kernel_info.version.revision_minor);
        Menus::DrawItem(2, "Hardware:", kernel_info.hardware_type);
        Menus::DrawItem(3, "Unit:", kernel_info.unit);
        Menus::DrawItem(4, "Serial:", kernel_info.serial_number.number);
        Menus::DrawItem(5, "DRAM ID:", kernel_info.dram_desc);
        Menus::DrawItemf(6, "Device ID:", "%llu", kernel_info.device_id);

        // if (hosversionAtLeast(2, 0, 0) && g_applet_operation_mode == AppletOperationMode_Console) {
        //     Menus::DrawItemf(7, "Dock firmware:", "%d.%d.%d.%d", kernel_info.dock_version.major, kernel_info.dock_version.minor, kernel_info.dock_version.micro, kernel_info.dock_version.rev);
        // }
    }

    static void SystemInfoPage(const SystemInfo& system_info) {
        s32 int_temp = SwitchIdent::GetBatteryTemperature(TsLocation_Internal);
        s32 ext_temp = SwitchIdent::GetBatteryTemperature(TsLocation_External);

        Menus::DrawItem(1, "Region:",  system_info.region);
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

    static void BatteryInfoPage(const BatteryInfo& battery_info) {
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
        Menus::DrawItem(5, "Battery lot number:", battery_info.battery_lot.lot);
        Menus::DrawItemf(6, "Charger current limit:",  "(In: %d mA) (Out: %d mA)", batteryChargeInfoFields.in_curr_limit, batteryChargeInfoFields.out_curr_limit);
        Menus::DrawItemf(7, "Battery charging current limit:",  "%d mA", batteryChargeInfoFields.charge_curr_limit);
        Menus::DrawItemf(8, "Battery charging voltage  limit:",  "%d mV", batteryChargeInfoFields.charge_volt_limit);
        Menus::DrawItemf(9, "Battery capacity:",  "%d pcm (Age: %d pcm)", batteryChargeInfoFields.capacity, batteryChargeInfoFields.battery_age);
        Menus::DrawItem(10, "Charger:", chargers[batteryChargeInfoFields.charger]);
        Menus::DrawItemf(11, "Charger voltage limit:", "%d mV", batteryChargeInfoFields.charger_volt_limit);
        Menus::DrawItemf(12, "Charger current limit:", "%d mA", batteryChargeInfoFields.charger_curr_limit);
    }

    static void StorageInfoPage(const StorageInfo& storage_info) {
        GUI::DrawRect(400, 50, 880, 670, bg_colour);
        
        GUI::DrawImage(drive, 450, 88);
        GUI::DrawRect(450, 226, 128, 25, descr_colour);
        GUI::DrawRect(452, 228, 124, 21, bg_colour);
        GUI::DrawRect(452, 228, ((static_cast<float>(storage_info.sd_used) / static_cast<float>(storage_info.sd_total)) * 124.f), 21, selector_colour);
        
        GUI::DrawImage(drive, 450, 296);
        GUI::DrawRect(450, 434, 128, 25, descr_colour);
        GUI::DrawRect(452, 436, 124, 21, bg_colour);
        GUI::DrawRect(452, 436, ((static_cast<float>(storage_info.nand_u_used) / static_cast<float>(storage_info.nand_u_total)) * 124.f), 21, selector_colour);
        
        GUI::DrawImage(drive, 450, 504);
        GUI::DrawRect(450, 642, 128, 25, descr_colour);
        GUI::DrawRect(452, 644, 124, 21, bg_colour);
        GUI::DrawRect(452, 644, ((static_cast<float>(storage_info.nand_s_used) / static_cast<float>(storage_info.nand_s_total)) * 124.f), 21, selector_colour);
        
        GUI::DrawText(600, 38 + ((g_item_dist - g_item_height) / 2) + 50, 25, descr_colour, "SD");
        Menus::DrawItem(600, 38 + ((g_item_dist - g_item_height) / 2) + 88, "Total storage capacity:", storage_info.sd_total_str);
        Menus::DrawItem(600, 38 + ((g_item_dist - g_item_height) / 2) + 126, "Free storage capacity:", storage_info.sd_free_str);
        Menus::DrawItem(600, 38 + ((g_item_dist - g_item_height) / 2) + 164, "Used storage capacity:", storage_info.sd_used_str);
        
        GUI::DrawText(600, 246 + ((g_item_dist - g_item_height) / 2) + 50, 25, descr_colour, "NAND User");
        Menus::DrawItem(600, 246 + ((g_item_dist - g_item_height) / 2) + 88, "Total storage capacity:", storage_info.nand_u_total_str);
        Menus::DrawItem(600, 246 + ((g_item_dist - g_item_height) / 2) + 126, "Free storage capacity:", storage_info.nand_u_free_str);
        Menus::DrawItem(600, 246 + ((g_item_dist - g_item_height) / 2) + 164, "Used storage capacity:", storage_info.nand_u_used_str);
        
        GUI::DrawText(600, 454 + ((g_item_dist - g_item_height) / 2) + 50, 25, descr_colour, "NAND System");
        Menus::DrawItem(600, 454 + ((g_item_dist - g_item_height) / 2) + 88, "Total storage capacity:", storage_info.nand_s_total_str);
        Menus::DrawItem(600, 454 + ((g_item_dist - g_item_height) / 2) + 126, "Free storage capacity:", storage_info.nand_s_free_str);
        Menus::DrawItem(600, 454 + ((g_item_dist - g_item_height) / 2) + 164, "Used storage capacity:", storage_info.nand_s_used_str);
    }

    static void JoyconInfoPage(JoyconInfo& joycon_info) {
        // TODO: Get info on other connected controllers
        HidPowerInfo info_left = SwitchIdent::GetJoyconPowerInfoL(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1);
        HidPowerInfo info_right = SwitchIdent::GetJoyconPowerInfoR(padIsHandheld(&g_pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1);

        Menus::DrawItemf(1, "Left Joy-con battery:", "%lu %% (%s)", (info_left.battery_level * 25), info_left.is_charging? "charging" : "not charging");
        Menus::DrawItemf(2, "Right Joy-con battery:", "%lu %% (%s)", (info_right.battery_level * 25), info_right.is_charging? "charging" : "not charging");
        //Menus::DrawItemf(3, "padIsHandheld:", "%s", padIsHandheld(&g_pad)? "true" : "false");

        if (!padIsHandheld(&g_pad)) {
            SwitchIdent::GetJoyconFirmwareVersion(HidDeviceTypeBits_JoyLeft, &joycon_info.version_left);
            SwitchIdent::GetJoyconFirmwareVersion(HidDeviceTypeBits_JoyRight, &joycon_info.version_right);
            Menus::DrawItemf(3, "Left Joy-con firmware:", "%d.%d.%d", joycon_info.version_left.major, joycon_info.version_left.minor, joycon_info.version_left.micro);
            Menus::DrawItemf(4, "Right Joy-con firmware:", "%d.%d.%d", joycon_info.version_right.major, joycon_info.version_right.minor, joycon_info.version_right.micro);
        }
        else {
            Menus::DrawItem(3, "Remove joy-cons from rail to read joy-con firmware.", " ");
        }
    }

    static void MiscInfoPage(const MiscInfo& misc_info) {
        char hostname[128];
        Result ret = gethostname(hostname, sizeof(hostname));

        Menus::DrawItem(1, "IP:",  R_SUCCEEDED(ret)? hostname : nullptr);
        Menus::DrawItem(2, "State:", SwitchIdent::GetOperationMode());
        Menus::DrawItem(3, "Automatic update:", SwitchIdent::GetAutoUpdateEnableFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(4, "Console information upload:", SwitchIdent::GetConsoleInformationUploadFlag()? "Enabled" : "Disabled");
        Menus::DrawItem(5, "SD card status:", misc_info.sd_inserted? "Inserted" : "Not inserted");
        Menus::DrawItem(6, "Game card status:", misc_info.gamecard_inserted? "Inserted" : "Not inserted");
        Menus::DrawItemf(7, "BT address:", "%02X:%02X:%02X:%02X:%02X:%02X", misc_info.bd_addr.bd_addr[0], misc_info.bd_addr.bd_addr[1], misc_info.bd_addr.bd_addr[2], misc_info.bd_addr.bd_addr[3], misc_info.bd_addr.bd_addr[4], misc_info.bd_addr.bd_addr[5]);
        Menus::DrawItemf(8, "WLAN address:", "%02X:%02X:%02X:%02X:%02X:%02X", misc_info.mac_addr.addr[0], misc_info.mac_addr.addr[1], misc_info.mac_addr.addr[2], misc_info.mac_addr.addr[3], misc_info.mac_addr.addr[4], misc_info.mac_addr.addr[5]);
    }

    void Main(void) {
        int title_height = 0;
        GUI::GetTextDimensions(25, "SwitchIdent", nullptr, &title_height);
        GUI::GetTextDimensions(25, "Item", nullptr, &g_item_height);
        
        int banner_width = 200;
        int selection = KERNEL_INFO_PAGE;
        
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

        KernelInfo kernel_info = { 0 };
        kernel_info.version = SwitchIdent::GetFirmwareVersion();
        kernel_info.dock_version = SwitchIdent::GetDockFirmware();
        kernel_info.hardware_type = SwitchIdent::GetHardwareType();
        kernel_info.unit = SwitchIdent::GetUnit();
        kernel_info.serial_number = SwitchIdent::GetSerialNumber();
        kernel_info.dram_desc = SwitchIdent::GetDramDesc();
        kernel_info.device_id = SwitchIdent::GetDeviceID();

        SystemInfo system_info = { 0 };
        system_info.region = SwitchIdent::GetRegion();

        BatteryInfo battery_info = { 0 };
        battery_info.battery_lot = SwitchIdent::GetBatteryLot();

        StorageInfo storage_info = { 0 };
        storage_info.sd_used = SwitchIdent::GetUsedStorage(NcmStorageId_SdCard);
        storage_info.sd_total = SwitchIdent::GetTotalStorage(NcmStorageId_SdCard);
        SwitchIdent::GetSizeString(storage_info.sd_total_str, storage_info.sd_total);
        SwitchIdent::GetSizeString(storage_info.sd_free_str, SwitchIdent::GetFreeStorage(NcmStorageId_SdCard));
        SwitchIdent::GetSizeString(storage_info.sd_used_str, storage_info.sd_used);
        storage_info.nand_u_used = SwitchIdent::GetUsedStorage(NcmStorageId_BuiltInUser);
        storage_info.nand_u_total = SwitchIdent::GetTotalStorage(NcmStorageId_BuiltInUser);
        SwitchIdent::GetSizeString(storage_info.nand_u_total_str, storage_info.nand_u_total);
        SwitchIdent::GetSizeString(storage_info.nand_u_free_str, SwitchIdent::GetFreeStorage(NcmStorageId_BuiltInUser));
        SwitchIdent::GetSizeString(storage_info.nand_u_used_str, storage_info.nand_u_used);
        storage_info.nand_s_used = SwitchIdent::GetUsedStorage(NcmStorageId_BuiltInSystem);
        storage_info.nand_s_total = SwitchIdent::GetTotalStorage(NcmStorageId_BuiltInSystem);
        SwitchIdent::GetSizeString(storage_info.nand_s_total_str, storage_info.nand_s_total);
        SwitchIdent::GetSizeString(storage_info.nand_s_free_str, SwitchIdent::GetFreeStorage(NcmStorageId_BuiltInSystem));
        SwitchIdent::GetSizeString(storage_info.nand_s_used_str, storage_info.nand_s_used);

        JoyconInfo joycon_info = { 0 };
        SwitchIdent::GetJoyconFirmwareVersion(HidDeviceTypeBits_JoyLeft, &joycon_info.version_left);
        SwitchIdent::GetJoyconFirmwareVersion(HidDeviceTypeBits_JoyRight, &joycon_info.version_right);
        
        MiscInfo misc_info = { 0 };
        Result ret = 0;
        
        FsDeviceOperator fsDeviceOperator;
        if (R_FAILED(ret = fsOpenDeviceOperator(&fsDeviceOperator))) {
            std::printf("fsOpenDeviceOperator() failed: 0x%x.\n\n", ret);
        }
            
        misc_info.sd_inserted = SwitchIdent::IsSDCardInserted(&fsDeviceOperator);
        misc_info.gamecard_inserted = SwitchIdent::IsGameCardInserted(&fsDeviceOperator);
        fsDeviceOperatorClose(&fsDeviceOperator);

        misc_info.bd_addr = SwitchIdent::GetBluetoothBdAddress();
        misc_info.mac_addr = SwitchIdent::GetWirelessLanMacAddress();

        while(appletMainLoop()) {
            GUI::ClearScreen(bg_colour);
            GUI::DrawRect(0, 0, 1920, 50, status_bar_colour);
            GUI::DrawRect(0, 50, 400, 1030, menu_bar_colour);
            
            GUI::DrawTextf(30, ((50 - title_height) / 2), 25, title_colour, "SwitchIdent v%d.%d", VERSION_MAJOR, VERSION_MINOR);
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
                
            if (selection > EXIT_PAGE) {
                selection = 0;
            }
            if (selection < 0) {
                selection = EXIT_PAGE;
            }

            switch (selection) {
                case KERNEL_INFO_PAGE:
                    Menus::KernelInfoPage(kernel_info);
                    break;
                
                case SYSTEM_INFO_PAGE:
                    Menus::SystemInfoPage(system_info);
                    break;
                
                case BATTERY_INFO_PAGE:
                    Menus::BatteryInfoPage(battery_info);
                    break;
                
                case STORAGE_INFO_PAGE:
                    Menus::StorageInfoPage(storage_info);
                    break;

                case JOYCON_INFO_PAGE:
                    Menus::JoyconInfoPage(joycon_info);
                    break;
                    
                case MISC_INFO_PAGE:
                    Menus::MiscInfoPage(misc_info);
                    break;

                default:
                    break;
            }
            
            GUI::Render();
            
            if ((kDown & HidNpadButton_Plus) || ((kDown & HidNpadButton_A) && (selection == EXIT_PAGE))) {
                break;
            }
        }
    }
}
