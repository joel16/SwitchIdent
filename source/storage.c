#include <stdio.h>

#include "storage.h"

u64 SwitchIdent_GetTotalStorage(FsStorageId storage_id)
{
	Result ret = 0;
	u64 total = 0;

	if (R_FAILED(ret = nsGetTotalSpaceSize(storage_id, &total)))
		printf("nsGetFreeSpaceSize() failed: 0x%x.\n\n", ret);

	return total;
}

u64 SwitchIdent_GetFreeStorage(FsStorageId storage_id)
{
	Result ret = 0;
	u64 free = 0;

	if (R_FAILED(ret = nsGetFreeSpaceSize(storage_id, &free)))
		printf("nsGetFreeSpaceSize() failed: 0x%x.\n\n", ret);

	return free;
}

u64 SwitchIdent_GetUsedStorage(FsStorageId storage_id)
{
	return (SwitchIdent_GetTotalStorage(storage_id) - SwitchIdent_GetFreeStorage(storage_id));
}