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

static Result psmGetBatteryVoltageState(Service *srv, u32 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 12;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u32 voltage;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->voltage;
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
        "max77620_sd0",
        "max77620_sd1",
        "max77620_sd2",
        "max77620_sd3",
        "max77620_ldo0 -> 1.2 V",
        "max77620_ldo1",
        "max77620_ldo2 -> 3.3 V - 1.8 V",
        "max77620_ldo3",
        "max77620_ldo4 -> 0.85 V",
        "max77620_ldo5",
        "max77620_ldo6 -> 2.9 V",
        "max77620_ldo7",
        "max77620_ldo8 -> 1.05 V",
        "max77621_cpu",
        "max77621_gpu",
        "Unknown"
    };

    if (R_SUCCEEDED(ret = psmGetBatteryVoltageState(srv, &out))) {
        if (out < 15)
            return states[out];
    }

    printf("psmGetBatteryVoltageState() failed: 0x%x.\n\n", ret);
    return states[15];
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
