#include <stdio.h>
#include <switch.h>

#include "setcal.h"


static Result psmIsBatteryChargingEnabled(u8 *out) {
    return serviceDispatchOut(psmGetServiceSession(), 4, *out);
}

static Result psmGetRawBatteryChargePercentage(u64 *out) {
    return serviceDispatchOut(psmGetServiceSession(), 13, *out);
}

static Result psmIsEnoughPowerSupplied(u8 *out) {
    return serviceDispatchOut(psmGetServiceSession(), 14, *out);
}

static Result psmGetBatteryAgePercentage(u64 *out) {
    return serviceDispatchOut(psmGetServiceSession(), 15, *out);
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

u8 SwitchIdent_IsChargingEnabled(void) {
	Result ret = 0;
	u8 out = 0;

	if (R_FAILED(ret = psmIsBatteryChargingEnabled(&out)))
		return -1;

	return out;
}

char *SwitchIdent_GetVoltageState(void) {
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

u64 SwitchIdent_GetRawBatteryChargePercentage(void) {
	Result ret = 0;
	u64 out = 0;

	if (R_FAILED(ret = psmGetRawBatteryChargePercentage(&out)))
		return -1;

	return out;
}

u8 SwitchIdent_IsEnoughPowerSupplied(void) {
	Result ret = 0;
	u8 out = 0;

	if (R_FAILED(ret = psmIsEnoughPowerSupplied(&out)))
		return -1;

	return out;
}

u64 SwitchIdent_GetBatteryAgePercent(void) {
	Result ret = 0;
	u64 out = 0;

	if (R_FAILED(ret = psmGetBatteryAgePercentage(&out))){
        printf("psmGetBatteryAgePercentage() failed: 0x%x.\n\n", ret);
		return -1;
    }

	return out;
}

SetBatteryLot SwitchIdent_GetBatteryLot(void) {
    Result ret = 0;
    SetBatteryLot lot = {0};

    if (R_FAILED(ret = setcalGetBatteryLot(&lot))) {
        printf("setcalGetBatteryLot() failed: 0x%x.\n\n", ret);
        return lot;
    }

    return lot;
}
