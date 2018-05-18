#ifndef _SWITCHIDENT_SYSTEM_H_
#define _SWITCHIDENT_SYSTEM_H_

const char *SwitchIdent_GetLanguage(void);
const char *SwitchIdent_GetRegion(void); // Not sure about this yet.
u32 SwitchIdent_GetCPUClock(void);
u32 SwitchIdent_GetGPUClock(void);

#endif
