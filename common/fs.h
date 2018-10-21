#ifndef _SWITCHIDENT_FS_H_
#define _SWITCHIDENT_FS_H_

#include <switch.h>

bool SwitchIdent_IsSDCardInserted(FsDeviceOperator *fsDeviceOperator);
bool SwitchIdent_IsGameCardInserted(FsDeviceOperator *fsDeviceOperator);

#endif
