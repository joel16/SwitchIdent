#ifndef _SWITCHIDENT_POWER_H_
#define _SWITCHIDENT_POWER_H_

u32 SwitchIdent_GetBatteryPercent(void);
u32 SwitchIdent_GetChargerType(Service *srv);
bool SwitchIdent_IsChargingEnabled(Service *srv);
u32 SwitchIdent_GetVoltage(Service *srv);
u64 SwitchIdent_GetRawBatteryChargePercentage(Service *srv);
bool SwitchIdent_IsEnoughPowerSupplied(Service *srv);
u64 SwitchIdent_GetBatteryAgePercent(Service *srv);

#endif