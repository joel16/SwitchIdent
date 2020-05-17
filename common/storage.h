#ifndef _SWITCHIDENT_STORAGE_H_
#define _SWITCHIDENT_STORAGE_H_

#include <switch.h>

s64 SwitchIdent_GetTotalStorage(NcmStorageId storage_id);
s64 SwitchIdent_GetFreeStorage(NcmStorageId storage_id);
s64 SwitchIdent_GetUsedStorage(NcmStorageId storage_id);

#endif
