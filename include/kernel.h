#ifndef _SWITCHIDENT_KERNEL_H_
#define _SWITCHIDENT_KERNEL_H_

const char *SwitchIdent_GetVersion(void);
const char *SwitchIdent_GetHardwareType(void);
const char *SwitchIdent_GetUnit(void);
bool SwitchIdent_IsSafeMode(void);
const char *SwitchIdent_GetSerialNumber(void);

#endif
