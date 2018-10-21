#ifndef _SWITCHIDENT_POWER_H_
#define _SWITCHIDENT_POWER_H_

u32 SwitchIdent_GetBatteryPercent(void);
char *SwitchIdent_GetChargerType(void);
bool SwitchIdent_IsCharging(void);
bool SwitchIdent_IsChargingEnabled(Service *srv);
char *SwitchIdent_GetVoltageState(Service *srv);
u64 SwitchIdent_GetRawBatteryChargePercentage(Service *srv);
bool SwitchIdent_IsEnoughPowerSupplied(Service *srv);
u64 SwitchIdent_GetBatteryAgePercent(Service *srv);

#endif
