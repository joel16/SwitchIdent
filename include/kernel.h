#ifndef _SWITCHIDENT_KERNEL_H_
#define _SWITCHIDENT_KERNEL_H_

u64 SwitchIdent_GetDramID(void);
const char *SwitchIdent_GetVersion(void);
const char *SwitchIdent_GetHardwareType(void);
const char *SwitchIdent_GetUnit(void);
bool SwitchIdent_IsSafeMode(void);
u64 SwitchIdent_GetDeviceID(void);
const char *SwitchIdent_GetSerialNumber(void);

#endif
