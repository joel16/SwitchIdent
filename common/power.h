#ifndef _SWITCHIDENT_POWER_H_
#define _SWITCHIDENT_POWER_H_

u32 SwitchIdent_GetBatteryPercent(void);
char *SwitchIdent_GetChargerType(void);
bool SwitchIdent_IsCharging(void);
bool SwitchIdent_IsChargingEnabled(void);
char *SwitchIdent_GetVoltageState(void);
double SwitchIdent_GetRawBatteryChargePercentage(void);
bool SwitchIdent_IsEnoughPowerSupplied(void);
double SwitchIdent_GetBatteryAgePercent(void);
char *SwitchIdent_GetBatteryLot(void);

#endif
