#ifndef _SWITCHIDENT_MISC_H_
#define _SWITCHIDENT_MISC_H_

char *SwitchIdent_GetOperationMode(void);
bool SwitchIdent_IsWirelessLanEnabled();
bool SwitchIdent_IsBluetoothEnabled();
bool SwitchIdent_IsNfcEnabled();
bool SwitchIdent_IsSDCardInserted(FsDeviceOperator *fsDeviceOperator);
bool SwitchIdent_IsGameCardInserted(FsDeviceOperator *fsDeviceOperator);
bool SwitchIdent_IsConsoleUpdateEnabled();
bool SwitchIdent_AutoUploadInformationEnabled();

#endif
