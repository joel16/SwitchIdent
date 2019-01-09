#ifndef _SWITCHIDENT_POWER_H_
#define _SWITCHIDENT_POWER_H_

Result powerInitialize(void);
void powerExit(void);
u32 SwitchIdent_GetBatteryPercent(void);
char *SwitchIdent_GetChargerType(void);
bool SwitchIdent_IsCharging(void);
bool SwitchIdent_IsChargingEnabled(void);
char *SwitchIdent_GetVoltageState(void);
u64 SwitchIdent_GetRawBatteryChargePercentage(void);
bool SwitchIdent_IsEnoughPowerSupplied(void);
u64 SwitchIdent_GetBatteryAgePercent(void);
char *SwitchIdent_GetBatteryLot(void);

#endif
