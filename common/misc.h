#ifndef _SWITCHIDENT_MISC_H_
#define _SWITCHIDENT_MISC_H_

char *SwitchIdent_GetOperationMode(void);
bool SwitchIdent_GetWirelessLanEnableFlag();
bool SwitchIdent_GetBluetoothEnableFlag();
bool SwitchIdent_GetNfcEnableFlag();
bool SwitchIdent_GetAutoUpdateEnableFlag();
bool SwitchIdent_GetConsoleInformationUploadFlag();
bool SwitchIdent_IsSDCardInserted(FsDeviceOperator *fsDeviceOperator);
bool SwitchIdent_IsGameCardInserted(FsDeviceOperator *fsDeviceOperator);

#endif
