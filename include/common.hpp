#pragma once

#include <switch.h>

typedef struct {
    u32 in_curr_limit;
    u32 out_curr_limit;
    u32 charge_curr_limit;
    u32 charge_volt_limit;
    u32 power_role;
    u32 enable_battery_charge;
    u32 charge_limits_temp;
    u32 battery_temp;
    u32 capacity;
    u32 voltage_avg;
    u32 battery_age;
    u32 power_role_2;
    u32 charger;
    u32 charger_volt_limit;
    u32 charger_curr_limit;
    u32 charge_method;
} BatteryChargeInfoFields;

typedef struct {
    u8 major;
    u8 minor;
    u8 micro;
    u8 rev;
    u8 device_identifier[0xC];
} HIDFirmwareVersion;

typedef struct {
    u32 major;
    u32 minor;
    u32 micro;
    u32 rev;
} DockFirmwareVersion;

namespace SwitchIdent {
    // Kernel
    const char *GetDramDesc(void);
    SetSysFirmwareVersion GetFirmwareVersion(void);
    const char *GetHardwareType(void);
    bool IsKiosk(void);
    const char *GetUnit(void);
    bool IsSafeMode(void);
    u64 GetDeviceID(void);
    SetSysSerialNumber GetSerialNumber(void);
    DockFirmwareVersion GetDockFirmware(void);

    // Misc
    const char *GetOperationMode(void);
    bool GetWirelessLanEnableFlag(void);
    bool GetBluetoothEnableFlag(void);
    bool GetNfcEnableFlag(void);
    bool GetAutoUpdateEnableFlag(void);
    bool GetConsoleInformationUploadFlag(void);
    bool IsSDCardInserted(FsDeviceOperator *fsDeviceOperator);
    bool IsGameCardInserted(FsDeviceOperator *fsDeviceOperator);

    // Power
    Result TsInitialize(void);
    u32 GetBatteryPercentage(void);
    const char *GetChargerType(void);
    bool IsCharging(void);
    bool IsChargingEnabled(void);
    const char *GetVoltageState(void);
    double GetRawBatteryChargePercentage(void);
    bool IsEnoughPowerSupplied(void);
    double GetBatteryAgePercentage(void);
    SetBatteryLot GetBatteryLot(void);
    s32 GetBatteryTemperature(TsLocation location);
    s32 GetBatteryChargeInfoFields(BatteryChargeInfoFields *batteryChargeInfoFields);

    // Storage
    s64 GetTotalStorage(NcmStorageId storage_id);
    s64 GetFreeStorage(NcmStorageId storage_id);
    s64 GetUsedStorage(NcmStorageId storage_id);
    void GetSizeString(char *string, double size);

    // System
    u64 GetLanguage(void);
    const char *GetRegion(void);
    u32 GetClock(PcvModule module);
    SetCalBdAddress GetBluetoothBdAddress(void);
    SetCalMacAddress GetWirelessLanMacAddress(void);
    const char *GetHeadphoneStatus(void);

    // Wlan
    u32 GetWlanState(void);
    s32 GetWlanQuality(s32 dBm);
    u32 GetWlanRSSI(void);

    // Joycon
    Result GetJoyconFirmwareVersion(HidDeviceTypeBits deviceType, HIDFirmwareVersion *version);
    HidPowerInfo GetJoyconPowerInfo(HidNpadIdType id);
    HidPowerInfo GetJoyconPowerInfoL(HidNpadIdType id);
    HidPowerInfo GetJoyconPowerInfoR(HidNpadIdType id);
}
