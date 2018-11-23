#ifndef _SWITCHIDENT_SYSTEM_H_
#define _SWITCHIDENT_SYSTEM_H_

char *SwitchIdent_GetLanguage(void);
char *SwitchIdent_GetRegion(void);
u32 SwitchIdent_GetCPUClock(void);
u32 SwitchIdent_GetGPUClock(void);
u32 SwitchIdent_GetEMCClock(void);
char *SwitchIdent_GetBluetoothBdAddress(Service *srv);
char *SwitchIdent_GetWirelessLanMacAddress(Service *srv);

#endif
