#ifndef _SWITCHIDENT_POWER_H_
#define _SWITCHIDENT_POWER_H_

u32 SwitchIdent_GetBatteryPercent(void);
char *SwitchIdent_GetChargerType(void);
bool SwitchIdent_IsCharging(void);
bool SwitchIdent_IsChargingEnabled(void);
char *SwitchIdent_GetVoltageState(void);
u64 SwitchIdent_GetRawBatteryChargePercentage(void);
bool SwitchIdent_IsEnoughPowerSupplied(void);
u64 SwitchIdent_GetBatteryAgePercent(void);
SetBatteryLot SwitchIdent_GetBatteryLot(void);

#endif
