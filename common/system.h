#ifndef _SWITCHIDENT_SYSTEM_H_
#define _SWITCHIDENT_SYSTEM_H_

char *SwitchIdent_GetLanguage(void);
char *SwitchIdent_GetRegion(void); // Not sure about this yet.
u32 SwitchIdent_GetCPUClock(void);
u32 SwitchIdent_GetGPUClock(void);

#endif
