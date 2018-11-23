#ifndef _SWITCHIDENT_MISC_H_
#define _SWITCHIDENT_MISC_H_

char *SwitchIdent_GetOperationMode(void);
bool SwitchIdent_GetFlag(SetSysFlag flag);
bool SwitchIdent_IsSDCardInserted(FsDeviceOperator *fsDeviceOperator);
bool SwitchIdent_IsGameCardInserted(FsDeviceOperator *fsDeviceOperator);

#endif
