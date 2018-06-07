#ifndef _SWITCHIDENT_MISC_H_
#define _SWITCHIDENT_MISC_H_

char *SwitchIdent_GetOperationMode(void);
bool SwitchIdent_GetWLANStatus(void);
bool SwitchIdent_GetBluetoothStatus(void);
bool SwitchIdent_GetNFCStatus(void);
bool SwitchIdent_GetLockscreenStatus(void);
bool SwitchIdent_GetAutoAppDownloadStatus(void);
bool SwitchIdent_GetConsoleInfoUploadStatus(void);

#endif
