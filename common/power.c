#include <stdio.h>

#include <switch.h>

#include "power.h"

static Result psmIsBatteryChargingEnabled(Service *srv, bool *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 4;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u8 enable;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->enable;
        }
    }
    
    return rc;
}

static Result psmGetRawBatteryChargePercentage(Service *srv, u64 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 13;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u64 age_percentage;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->age_percentage;
        }
    }
    
    return rc;
}

static Result psmIsEnoughPowerSupplied(Service *srv, bool *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 14;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u8 power_supplied;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->power_supplied;
        }
    }
    
    return rc;
}

static Result psmGetBatteryAgePercentage(Service *srv, u64 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 15;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u64 age_percentage;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->age_percentage;
        }
    }
    
    return rc;
}

u32 SwitchIdent_GetBatteryPercent(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = psmGetBatteryChargePercentage(&out)))
		return -1;

	return out;
}

char *SwitchIdent_GetChargerType(void) {
	Result ret = 0;
	ChargerType charger_type;

	if (R_FAILED(ret = psmGetChargerType(&charger_type)))
		return NULL;

    if (charger_type == ChargerType_Charger)
        return "Official charger or dock";
    else if (charger_type == ChargerType_Usb)
        return "USB-C charger";
    else
        return "No charger connected";

    return NULL;
}

bool SwitchIdent_IsCharging(void) {
    Result ret = 0;
    ChargerType charger_type;

    if (R_FAILED(ret = psmGetChargerType(&charger_type)))
        return false;

    if ((charger_type == ChargerType_Charger) || (charger_type == ChargerType_Usb))
        return true;

    return false;
}

bool SwitchIdent_IsChargingEnabled(Service *srv) {
	Result ret = 0;
	bool out = 0;

	if (R_FAILED(ret = psmIsBatteryChargingEnabled(srv, &out)))
		return -1;

	return out;
}

char *SwitchIdent_GetVoltageState(Service *srv) {
	Result ret = 0;
	u32 out = 0;

    char *states[]=
    {
        "Power state needs shutdown",
        "Power state needs sleep",
        "Performance boost cannot be entered",
        "Normal",
        "Unknown"
    };

    if (R_SUCCEEDED(ret = psmGetBatteryVoltageState(&out))) {
        if (out < 4)
            return states[out];
    }

    printf("psmGetBatteryVoltageState() failed: 0x%x.\n\n", ret);
    return states[4];
}

u64 SwitchIdent_GetRawBatteryChargePercentage(Service *srv) {
	Result ret = 0;
	u64 out = 0;

	if (R_FAILED(ret = psmGetRawBatteryChargePercentage(srv, &out)))
		return -1;

	return out;
}

bool SwitchIdent_IsEnoughPowerSupplied(Service *srv) {
	Result ret = 0;
	bool out = 0;

	if (R_FAILED(ret = psmIsEnoughPowerSupplied(srv, &out)))
		return -1;

	return out;
}

u64 SwitchIdent_GetBatteryAgePercent(Service *srv) {
	Result ret = 0;
	u64 out = 0;

	if (R_FAILED(ret = psmGetBatteryAgePercentage(srv, &out)))
		return -1;

	return out;
}
