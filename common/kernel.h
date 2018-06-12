#ifndef _SWITCHIDENT_KERNEL_H_
#define _SWITCHIDENT_KERNEL_H_

char *SwitchIdent_GetDramDesc(void);
char *SwitchIdent_GetFirmwareVersion(Service *srv);
char *SwitchIdent_GetKernelVersion(void);
char *SwitchIdent_GetHardwareType(void);
char *SwitchIdent_GetUnit(void);
bool SwitchIdent_IsSafeMode(void);
u64 SwitchIdent_GetDeviceID(void);
char *SwitchIdent_GetSerialNumber(void);

#endif
