#include <stdio.h>
#include <switch.h>

#include "kernel.h"

static u64 SwitchIdent_GetPackage1MaxverConstant(void)
{
	Result ret = 0;
	u64 version = 0x0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_Version, &version)))
		printf("splGetConfig(SplConfigItem_Version) failed: 0x%x.\n\n", ret);

	return (version + 1); // (Package1 maxver constant - 1) + 1
}

const char *SwitchIdent_GetVersion(void)
{
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

	return versions[SwitchIdent_GetPackage1MaxverConstant()];
}

const char *SwitchIdent_GetHardwareType(void)
{
	Result ret = 0;
	u64 hardware_type = 4;

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

	if (SwitchIdent_GetPackage1MaxverConstant() < 0x6)
		return hardware_300[hardware_type];

	return hardware_400[hardware_type];
}

// [4.0.0+] Kiosk mode (0 = retail; 1 = kiosk)
static bool SwitchIdent_IsKiosk(void)
{
	u64 isKiosk = 0;
	Result ret = 0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsKiosk , &isKiosk)))
		printf("splGetConfig(SplConfigItem_IsKiosk ) failed: 0x%x.\n\n", ret);

	if (isKiosk) // if bit 10 returns (1)
		return true;

	return false; 
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

	if (SwitchIdent_GetPackage1MaxverConstant() >= 0x6) // 4.00+
	{
		if (SwitchIdent_IsKiosk())
			return "Kiosk";
	}

	return unit[isRetail];
}

bool SwitchIdent_IsSafeMode(void)
{
	Result ret = 0;
	u64 safemode = 0;
	bool out = false;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRecoveryBoot, &safemode)))
		printf("splGetConfig(SplConfigItem_IsRecoveryBoot) failed: 0x%x.\n\n", ret);

	out = safemode; // 1 = true, 0 = false

	return out;
}

u64 SwitchIdent_GetDeviceID(void)
{
	Result ret = 0;
	u64 deviceID = 0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_DeviceId, &deviceID)))
		printf("splGetConfig(SplConfigItem_DeviceId) failed: 0x%x.\n\n", ret);

	return deviceID;
}

const char *SwitchIdent_GetSerialNumber(void)
{
	Result ret = 0;
	static char serial[0x19];

	if (R_FAILED(ret = setsysGetSerialNumber(serial)))
		printf("setsysGetSerialNumber() failed: 0x%x.\n\n", ret);

	return serial;
}
