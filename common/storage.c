#include <stdio.h>

#include "storage.h"

s64 SwitchIdent_GetTotalStorage(NcmStorageId storage_id) {
	Result ret = 0;
	s64 total = 0;

	if (R_FAILED(ret = nsGetTotalSpaceSize(storage_id, &total)))
		printf("nsGetFreeSpaceSize() failed: 0x%x.\n\n", ret);

	return total;
}

s64 SwitchIdent_GetFreeStorage(NcmStorageId storage_id) {
	Result ret = 0;
	s64 free = 0;

	if (R_FAILED(ret = nsGetFreeSpaceSize(storage_id, &free)))
		printf("nsGetFreeSpaceSize() failed: 0x%x.\n\n", ret);

	return free;
}

s64 SwitchIdent_GetUsedStorage(NcmStorageId storage_id) {
	return (SwitchIdent_GetTotalStorage(storage_id) - SwitchIdent_GetFreeStorage(storage_id));
}
