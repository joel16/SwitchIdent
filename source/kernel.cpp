#include <cstdio>

#include "common.hpp"

namespace SwitchIdent {
    const char *GetDramDesc(void) {
        Result ret = 0;
        u64 id = 0;
        
        const char *dram_desc[] = {
            "EristaIcosaSamsung4gb",
            "EristaIcosaHynix4gb",
            "EristaIcosaMicron4gb",
            "MarikoIowaHynix1y4gb",
            "EristaIcosaSamsung6gb",
            "MarikoHoagHynix1y4gb",
            "MarikoAulaHynix1y4gb",
            "MarikoIowax1x2Samsung4gb",
            "MarikoIowaSamsung4gb",
            "MarikoIowaSamsung8gb",
            "MarikoIowaHynix4gb",
            "MarikoIowaMicron4gb",
            "MarikoHoagSamsung4gb",
            "MarikoHoagSamsung8gb",
            "MarikoHoagSamsung8gb",
            "MarikoHoagHynix4gb",
            "MarikoHoagMicron4gb",
            "MarikoIowaSamsung4gbY",
            "MarikoIowaSamsung1y4gbX",
            "MarikoIowaSamsung1y8gbX",
            "MarikoHoagSamsung1y4gbX",
            "MarikoIowaSamsung1y4gbY",
            "MarikoIowaSamsung1y8gbY",
            "MarikoAulaSamsung1y4gb",
            "MarikoHoagSamsung1y8gbX",
            "MarikoAulaSamsung1y4gbX",
            "MarikoIowaMicron1y4gb",
            "MarikoHoagMicron1y4gb",
            "MarikoAulaMicron1y4gb",
            "MarikoAulaSamsung1y8gbX",
            "Unknown"
        };
        
        if (R_FAILED(ret = splGetConfig(SplConfigItem_DramId, &id)))
            std::printf("splGetConfig(SplConfigItem_DramId) failed: 0x%x.\n\n", ret);
            
        if (id >= 30)
            return dram_desc[30];
            
        return dram_desc[id];
    }
    
    SetSysFirmwareVersion GetFirmwareVersion(void) {
        Result ret = 0;
        SetSysFirmwareVersion version;
        
        if (R_FAILED(ret = setsysGetFirmwareVersion(&version)))
            std::printf("setsysGetFirmwareVersion() failed: 0x%x.\n\n", ret);

        return version;
    }
    
    const char *GetHardwareType(void) {
        Result ret = 0;
        u64 hardware_type;
        
        const char *hardware_string[] = {
            "Icosa",
            "Copper",
            "Hoag",
            "Iowa",
            "Calcio",
            "Aula",
            "Unknown"
        };
        
        if (R_FAILED(ret = splGetConfig(SplConfigItem_HardwareType, &hardware_type)))
            std::printf("splGetConfig(SplConfigItem_HardwareType) failed: 0x%x.\n\n", ret);
            
        if (hardware_type >= 6)
            return hardware_string[6];
            
        return hardware_string[hardware_type];
    }
    
    // [4.0.0+] Kiosk mode (0 = retail; 1 = kiosk)
    bool IsKiosk(void) {
        u64 is_kiosk_mode = 0;
        Result ret = 0;
        
        if (R_FAILED(ret = splGetConfig(SplConfigItem_IsKiosk , &is_kiosk_mode)))
            std::printf("splGetConfig(SplConfigItem_IsKiosk) failed: 0x%x.\n\n", ret);
        
        return is_kiosk_mode? true : false;
    }
    
    const char *GetUnit(void) {
        Result ret = 0;
        u64 is_retail_mode = 0;
        
        const char *unit[] = {
            "Debug",
            "Retail",
            "Unknown"
        };
        
        if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRetail, &is_retail_mode))) {
            std::printf("splGetConfig(SplConfigItem_IsRetail) failed: 0x%x.\n\n", ret);
            return unit[2];
        }
        
        return unit[is_retail_mode];
    }
    
    bool IsSafeMode(void) {
        Result ret = 0;
        u64 safemode = 0;
        
        if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRecoveryBoot, &safemode)))
            std::printf("splGetConfig(SplConfigItem_IsRecoveryBoot) failed: 0x%x.\n\n", ret);
            
        if (safemode)
            return true;
        
        return false;
    }
    
    u64 GetDeviceID(void) {
        Result ret = 0;
        u64 id = 0;
        
        if (R_FAILED(ret = splGetConfig(SplConfigItem_DeviceId, &id)))
            std::printf("splGetConfig(SplConfigItem_DeviceId) failed: 0x%x.\n\n", ret);
            
        return id;
    }
    
    SetSysSerialNumber GetSerialNumber(void) {
        Result ret = 0;
        SetSysSerialNumber serial;
        
        if (R_FAILED(ret = setsysGetSerialNumber(&serial)))
            std::printf("setsysGetSerialNumber() failed: 0x%x.\n\n", ret);
            
        return serial;
    }
}
