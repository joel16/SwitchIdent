#ifndef _SWITCHIDENT_SYSTEM_H_
#define _SWITCHIDENT_SYSTEM_H_

char *SwitchIdent_GetLanguage(void);
char *SwitchIdent_GetRegion(void);
u32 SwitchIdent_GetClock(PcvModule module);
char *SwitchIdent_GetBluetoothBdAddress(void);
char *SwitchIdent_GetWirelessLanMacAddress(void);

#endif
