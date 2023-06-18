#include <cstdio>
#include "common.hpp"

namespace SwitchIdent {
    u32 GetWlanState(void) {
        Result ret = 0;
        WlanInfState state;
        
        if (R_FAILED(ret = wlaninfGetState(&state))) {
            return -1;
        }
            
        return state;
    }
    
    s32 GetWlanQuality(s32 dBm) {
        u32 quality = 0;
        
        if (dBm <= -100) {
            quality = 0;
        }
        else if (dBm >= -50) {
            quality = 100;
        }
        else {
            quality = 2 * (dBm + 100);
        }
            
        return quality;
    }
    
    u32 GetWlanRSSI(void) {
        Result ret = 0;
        s32 rssi = 0;
        
        if (R_FAILED(ret = wlaninfGetRSSI(&rssi))) {
            return -1;
        }
        
        return rssi;
    }
}
