#include <stdio.h>
#include <switch.h>

#include "kernel.h"
#include "setsys.h"

static u64 SwitchIdent_GetPackage1MaxverConstant(void) {
	Result ret = 0;
	u64 version = 0x0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_Version, &version)))
		printf("splGetConfig(SplConfigItem_Version) failed: 0x%x.\n\n", ret);

	return (version + 1); // (Package1 maxver constant - 1) + 1
}

char *SwitchIdent_GetDramDesc(void) {
	Result ret = 0;
	u64 id = 0;

	char *dram_desc[] = {
		"EristaIcosaSamsung4gb",
		"EristaIcosaHynix4gb",
		"EristaIcosaMicron4gb",
		"Unknown",
		"EristaIcosaSamsung6gb",
		"Unknown",
		"Unknown",
		"MarikoIowax1x2Samsung4gb",
		"MarikoIowaSamsung4gb",
		"MarikoIowaSamsung8gb",
		"Unknown",
		"Unknown",
		"MarikoHoagSamsung4gb",
		"MarikoHoagSamsung8gb"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_DramId, &id)))
		printf("splGetConfig(SplConfigItem_DramId) failed: 0x%x.\n\n", ret);

	return dram_desc[id];
}

char *SwitchIdent_GetFirmwareVersion(void) {
	Result ret = 0;
	SetSysFirmwareVersion ver;

	if (R_FAILED(ret = setsysGetFirmwareVersion(&ver))) {
		printf("setsysGetFirmwareVersion() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	static char buf[9];
	snprintf(buf, 9, "%u.%u.%u-%u%u", ver.major, ver.minor, ver.micro, ver.revision_major, ver.revision_minor);

	return buf;
}

char *SwitchIdent_GetKernelVersion(void) {
	char *versions[] = {
		"Unknown0",
		"Unknown1",
		"5.0.0",
		"6.1.0",
		"7.4.0",
		"7.4.0",
		"X.X.X",
		"9.3.0",
		"Unknown2",
		"Unknown3"
	};

	return versions[SwitchIdent_GetPackage1MaxverConstant()];
}

char *SwitchIdent_GetHardwareType(void) {
	Result ret = 0;
	u64 hardware_type = 4;

	char *hardware_300[] = {
		"Icosa",
		"Copper",
		"Unknown",
		"Unknown",
		"Unknown"
	};

	char *hardware_400[] = {
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
static bool SwitchIdent_IsKiosk(void) {
	u64 isKiosk = 0;
	Result ret = 0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsKiosk , &isKiosk)))
		printf("splGetConfig(SplConfigItem_IsKiosk) failed: 0x%x.\n\n", ret);

	if (isKiosk) // if bit 10 returns (1)
		return true;

	return false; 
}

char *SwitchIdent_GetUnit(void) {
	Result ret = 0;
	u64 isRetail = 2;

	char *unit[] = {
		"Debug",
		"Retail",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRetail, &isRetail)))
		printf("splGetConfig(SplConfigItem_IsRetail) failed: 0x%x.\n\n", ret);

	if (SwitchIdent_GetPackage1MaxverConstant() >= 0x6) {// 4.00+
		if (SwitchIdent_IsKiosk())
			return "Kiosk";
	}

	return unit[isRetail];
}

bool SwitchIdent_IsSafeMode(void) {
	Result ret = 0;
	u64 safemode = 0;
	bool out = false;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRecoveryBoot, &safemode)))
		printf("splGetConfig(SplConfigItem_IsRecoveryBoot) failed: 0x%x.\n\n", ret);

	out = safemode; // 1 = true, 0 = false

	return out;
}

u64 SwitchIdent_GetDeviceID(void) {
	Result ret = 0;
	u64 deviceID = 0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_DeviceId, &deviceID)))
		printf("splGetConfig(SplConfigItem_DeviceId) failed: 0x%x.\n\n", ret);

	return deviceID;
}

char *SwitchIdent_GetSerialNumber(void) {
	Result ret = 0;
	static char serial[0x19];

	if (R_FAILED(ret = setsysGetSerialNumber(serial)))
		printf("setsysGetSerialNumber() failed: 0x%x.\n\n", ret);

	return serial;
}
