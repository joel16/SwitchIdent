#ifndef _SWITCHIDENT_COMMON_H_
#define _SWITCHIDENT_COMMON_H_

#include <switch.h>

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
    u32 GetBatteryPercentage(void);
    const char *GetChargerType(void);
    bool IsCharging(void);
    bool IsChargingEnabled(void);
    const char *GetVoltageState(void);
    double GetRawBatteryChargePercentage(void);
    bool IsEnoughPowerSupplied(void);
    double GetBatteryAgePercentage(void);
    SetBatteryLot GetBatteryLot(void);

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

    // Wlan
    u32 GetWlanState(void);
    s32 GetWlanQuality(s32 dBm);
    u32 GetWlanRSSI(void);
}

#endif
