#include <errno.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <switch.h>

#include "storage.h"

u64 SwitchIdent_GetTotalStorage(void)
{
	struct statvfs buf;

	if (R_FAILED(statvfs("/", &buf)))
	{
		printf("statvfs: %d\n", errno);
		return 0;
	}

	u64 total = buf.f_blocks * buf.f_frsize;
	return total;
}

u64 SwitchIdent_GetFreeStorage(void)
{
	struct statvfs buf;

	if (R_FAILED(statvfs("/", &buf)))
	{
		printf("statvfs: %d\n", errno);
		return 0;
	}

	u64 free = buf.f_bfree * buf.f_frsize;
	return free;
}

u64 SwitchIdent_GetUsedStorage(void)
{
	struct statvfs buf;

	if (R_FAILED(statvfs("/", &buf)))
	{
		printf("statvfs: %d\n", errno);
		return 0;
	}

	u64 used = (buf.f_blocks * buf.f_frsize) - (buf.f_bfree * buf.f_frsize);
	return used;
}