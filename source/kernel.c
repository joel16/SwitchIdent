#include <stdio.h>

#include <switch.h>

#include "kernel.h"

const char *SwitchIdent_GetVersion(void)
{
	Result ret = 0;
	u64 version = 8;

	const char *versions[] =
	{
		"Unknown0",
		"Unknown1",
		"1.0.0",     // 0x2
		"2.0.0",     // 0x3
		"3.0.0",     // 0x4
		"3.0.2",     // 0x5
		"4.0.0",     // 0x6
		"5.0.0",     // 0x7
		"Unknown2",
		"Unknown3"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_Version, &version)))
		printf("splGetConfig(SplConfigItem_Version) failed: 0x%x.\n\n", ret);

	return versions[version + 1]; // (Package1 maxver constant - 1) + 1
}

const char *SwitchIdent_GetHardwareType(void)
{
	Result ret = 0;
	u64 hardware_type = 4, version = 0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_Version, &version)))
		printf("splGetConfig(SplConfigItem_Version) failed: 0x%x.\n\n", ret);

	version = version + 1;

	const char *hardware_300[] =
	{
		"Icosa",
		"Copper",
		"Unknown",
		"Unknown",
		"Unknown"
	};

	const char *hardware_400[] =
	{
		"Icosa",
		"Copper",
		"Unknown",
		"Mariko",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_HardwareType, &hardware_type)))
		printf("splGetConfig(SplConfigItem_HardwareType) failed: 0x%x.\n\n", ret);

	if (version < 0x6)
		return hardware_300[hardware_type];
	
	return hardware_400[hardware_type];
}

const char *SwitchIdent_GetUnit(void)
{
	Result ret = 0;
	u64 isRetail = 2;

	const char *unit[] =
	{
		"Debug",
		"Retail",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRetail, &isRetail)))
		printf("splGetConfig(SplConfigItem_IsRetail) failed: 0x%x.\n\n", ret);

	return unit[isRetail];
}