#include <stdio.h>

#include <switch.h>

#include "kernel.h"

const char *SwitchIdent_GetVersion(void)
{
	Result ret = 0;
	u64 version = 8;

	const char *versions[] =
	{
		"",          // 0x0
		"",          // 0x1
		"",          // 0x2
		"1.0.0",     // 0x3
		"2.0.0",     // 0x4
		"3.0.0",     // 0x5
		"3.0.2",     // 0x6
		"4.0.0",     // 0x7
		"5.0.0"      // 0x8
		"Unknown"    // Couldn't get version
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_Version, &version)))
		printf("splGetConfig(SplConfigItem_Version) failed: 0x%x.\n\n", ret);

	return versions[version + 1];
}

const char *SwitchIdent_GetHardwareType(void)
{
	Result ret = 0;
	u64 hardware_type = 2;

	const char *hardware[] =
	{
		"Icosa",
		"Copper",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_HardwareType, &hardware_type)))
		printf("splGetConfig(SplConfigItem_HardwareType) failed: 0x%x.\n\n", ret);

	return hardware[hardware_type];
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