#include <stdio.h>
#include <switch.h>

static Result _psmCmdNoInOutBool(Service* srv, bool *out, u32 cmd_id) {
    u8 outval = 0;
    Result rc = serviceDispatchOut(srv, cmd_id, outval);
    if (R_SUCCEEDED(rc)) {
        if (out) *out = outval & 1;
    }
    return rc;
}

static Result psmIsBatteryChargingEnabled(bool *out) {
    return _psmCmdNoInOutBool(psmGetServiceSession(), out, 4);
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
	PsmChargerType charger_type;

	if (R_FAILED(ret = psmGetChargerType(&charger_type)))
		return NULL;

    if (charger_type == PsmChargerType_EnoughPower)
        return "Official charger or dock";
    else if (charger_type == PsmChargerType_LowPower)
        return "USB-C charger";
    else
        return "No charger connected";

    return NULL;
}

bool SwitchIdent_IsCharging(void) {
    Result ret = 0;
    PsmChargerType charger_type;

    if (R_FAILED(ret = psmGetChargerType(&charger_type)))
        return false;

    return charger_type != PsmChargerType_Unconnected;
}

bool SwitchIdent_IsChargingEnabled(void) {
	Result ret = 0;
	bool out = 0;

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

double SwitchIdent_GetRawBatteryChargePercentage(void) {
	Result ret = 0;
	double out = 0;

	if (R_FAILED(ret = psmGetRawBatteryChargePercentage(&out)))
		return -1;

	return out;
}

bool SwitchIdent_IsEnoughPowerSupplied(void) {
	Result ret = 0;
	bool out = 0;

	if (R_FAILED(ret = psmIsEnoughPowerSupplied(&out)))
		return -1;

	return out;
}

double SwitchIdent_GetBatteryAgePercent(void) {
	Result ret = 0;
	double out = 0;

	if (R_FAILED(ret = psmGetBatteryAgePercentage(&out)))
		return -1;

	return out;
}

char *SwitchIdent_GetBatteryLot(void) {
    Result ret = 0;
    static SetBatteryLot battery_lot;

    if (R_FAILED(ret = setcalGetBatteryLot(&battery_lot))) {
        printf("setcalGetBatteryLot() failed: 0x%x.\n\n", ret);
        return NULL;
    }

    return battery_lot.lot;
}
