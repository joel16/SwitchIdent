#ifndef _SWITCHIDENT_WLAN_H_
#define _SWITCHIDENT_WLAN_H_

Result wlaninfInitialize(void);
void wlaninfExit(void);
u32 SwitchIdent_GetWlanState(void);
u32 SwitchIdent_GetWlanQuality(u32 dBm);
u32 SwitchIdent_GetWlanRSSI(void);

#endif
