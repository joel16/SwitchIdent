#include <cstdio>

#include "common.hpp"

namespace SwitchIdent {
    u64 GetLanguage(void) {
        Result ret = 0;
        u64 language = 0;
        
        if (R_FAILED(ret = setGetSystemLanguage(&language))) {
            std::printf("setGetSystemLanguage() failed: 0x%x.\n\n", ret);
        }
            
        return language;
    }
    
    const char *GetRegion(void) {
        Result ret = 0;
        SetRegion region;
        const char *regions[] = {
            "JPN",
            "USA",
            "EUR",
            "AUS",
            "CHN",
            "KOR",
            "TWN",
            "Unknown"
        };
        
        if (R_FAILED(ret = setGetRegionCode(&region))) {
            std::printf("setGetRegionCode() failed: 0x%x.\n\n", ret);
            return regions[7];
        }
        
        return regions[region];
    }
    
    u32 GetClock(PcvModule module) {
        Result ret = 0;
        u32 out = 0;
        
        if (hosversionAtLeast(8, 0, 0)) {
            ClkrstSession session = {0};
            PcvModuleId module_id;
            
            if (R_FAILED(ret = pcvGetModuleId(&module_id, module))) {
                std::printf("pcvGetModuleId() failed: 0x%x.\n\n", ret);
            }
            else if (R_FAILED(ret = clkrstOpenSession(&session, module_id, 3))) {
                std::printf("clkrstOpenSession() failed: 0x%x.\n\n", ret);
            }
            else if (R_FAILED(ret = clkrstGetClockRate(&session, &out))) {
                std::printf("clkrstGetClockRate() failed: 0x%x.\n\n", ret);
            }
            else {
                clkrstCloseSession(&session);
            }
        }
        else {
            if (R_FAILED(ret = pcvGetClockRate(module, &out))) {
                std::printf("pcvGetClockRate() failed: 0x%x.\n\n", ret);
            }
        }
        
        return out/1000000;
    }
    
    SetCalBdAddress GetBluetoothBdAddress(void) {
        Result ret = 0;
        SetCalBdAddress bd_addr;
        
        if (R_FAILED(ret = setcalGetBdAddress(&bd_addr))) {
            std::printf("setcalGetBdAddress() failed: 0x%x.\n\n", ret);
        }
            
        return bd_addr;
    }
    
    SetCalMacAddress GetWirelessLanMacAddress(void) {
        Result ret = 0;
        SetCalMacAddress mac_addr;
        
        if (R_FAILED(ret = setcalGetWirelessLanMacAddress(&mac_addr))) {
            std::printf("setcalGetWirelessLanMacAddress() failed: 0x%x.\n\n", ret);
        }
            
        return mac_addr;
    }
}
