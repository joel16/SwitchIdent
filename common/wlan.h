#ifndef _SWITCHIDENT_WLAN_H_
#define _SWITCHIDENT_WLAN_H_

u32 SwitchIdent_GetWlanState(Service *srv);
u32 SwitchIdent_GetWlanQuality(u32 dBm);
u32 SwitchIdent_GetWlanRSSI(Service *srv);

#endif
