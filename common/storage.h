#ifndef _SWITCHIDENT_STORAGE_H_
#define _SWITCHIDENT_STORAGE_H_

#include <switch.h>

u64 SwitchIdent_GetTotalStorage(FsStorageId storage_id);
u64 SwitchIdent_GetFreeStorage(FsStorageId storage_id);
u64 SwitchIdent_GetUsedStorage(FsStorageId storage_id);

#endif
