#include <cstdio>
#include "common.hpp"

namespace SwitchIdent {
    
    static Result _psmCmdNoInOutBool(Service* srv, bool *out, u32 cmd_id) {
        u8 outval = 0;
        Result ret = serviceDispatchOut(srv, cmd_id, outval);
        if (R_SUCCEEDED(ret)) {
            if (out)
                *out = outval & 1;
        }
        
        return ret;
    }
    
    static Result psmIsBatteryChargingEnabled(bool *out) {
        return _psmCmdNoInOutBool(psmGetServiceSession(), out, 4);
    }
    
    u32 GetBatteryPercentage(void) {
        Result ret = 0;
        u32 percentage = 0;
        
        if (R_FAILED(ret = psmGetBatteryChargePercentage(&percentage)))
            return -1;
        
        return percentage;
    }
    
    const char *GetChargerType(void) {
        Result ret = 0;
        PsmChargerType charger_type;
        
        if (R_FAILED(ret = psmGetChargerType(&charger_type)))
            return nullptr;
            
        if (charger_type == PsmChargerType_EnoughPower)
            return "Official charger or dock";
        else if (charger_type == PsmChargerType_LowPower)
            return "USB-C charger";
        else
            return "No charger connected";
            
        return nullptr;
    }
    
    bool IsCharging(void) {
        Result ret = 0;
        PsmChargerType charger_type;
        
        if (R_FAILED(ret = psmGetChargerType(&charger_type)))
            return false;
            
        return charger_type != PsmChargerType_Unconnected;
    }
    
    bool IsChargingEnabled(void) {
        Result ret = 0;
        bool is_charing_enabled = 0;
        
        if (R_FAILED(ret = psmIsBatteryChargingEnabled(&is_charing_enabled)))
            return -1;
        
        return is_charing_enabled;
    }
    
    const char *GetVoltageState(void) {
        Result ret = 0;
        PsmBatteryVoltageState voltage_state;
        const char *states[] = {
            "Power state needs shutdown",
            "Power state needs sleep",
            "Performance boost cannot be entered",
            "Normal",
            "Unknown"
        };
        
        if (R_SUCCEEDED(ret = psmGetBatteryVoltageState(&voltage_state))) {
            if (voltage_state < 4)
                return states[voltage_state];
        }
        
        std::printf("psmGetBatteryVoltageState() failed: 0x%x.\n\n", ret);
        return states[4];
    }
    
    double GetRawBatteryChargePercentage(void) {
        Result ret = 0;
        double raw_percentage = 0;
        
        if (R_FAILED(ret = psmGetRawBatteryChargePercentage(&raw_percentage)))
            return -1;
            
        return raw_percentage;
    }
    
    bool IsEnoughPowerSupplied(void) {
        Result ret = 0;
        bool is_power_supplied = 0;
        
        if (R_FAILED(ret = psmIsEnoughPowerSupplied(&is_power_supplied)))
            return -1;
            
        return is_power_supplied;
    }
    
    double GetBatteryAgePercentage(void) {
        Result ret = 0;
        double age_percentage = 0;
        
        if (R_FAILED(ret = psmGetBatteryAgePercentage(&age_percentage)))
            return -1;
            
        return age_percentage;
    }
    
    SetBatteryLot GetBatteryLot(void) {
        Result ret = 0;
        SetBatteryLot battery_lot;
        
        if (R_FAILED(ret = setcalGetBatteryLot(&battery_lot)))
            std::printf("setcalGetBatteryLot() failed: 0x%x.\n\n", ret);
            
        return battery_lot;
    }
}
