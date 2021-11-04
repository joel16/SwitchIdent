#include <stdio.h>
#include <switch.h>

char *SwitchIdent_GetDramDesc(void) {
	Result ret = 0;
	u64 id = 0;

	char *dram_desc[] = {
		"EristaIcosaSamsung4gb",
		"EristaIcosaHynix4gb",
		"EristaIcosaMicron4gb",
		"MarikoIowaHynix1y4gb",
		"EristaIcosaSamsung6gb",
		"MarikoHoagHynix1y4gb",
		"MarikoAulaHynix1y4gb",
		"MarikoIowax1x2Samsung4gb",
		"MarikoIowaSamsung4gb",
		"MarikoIowaSamsung8gb",
		"MarikoIowaHynix4gb",
		"MarikoIowaMicron4gb",
		"MarikoHoagSamsung4gb",
		"MarikoHoagSamsung8gb",
		"MarikoHoagSamsung8gb",
		"MarikoHoagHynix4gb",
		"MarikoHoagMicron4gb",
		"MarikoIowaSamsung4gbY",
		"MarikoIowaSamsung1y4gbX",
		"MarikoIowaSamsung1y8gbX",
		"MarikoHoagSamsung1y4gbX",
		"MarikoIowaSamsung1y4gbY",
		"MarikoIowaSamsung1y8gbY",
		"MarikoAulaSamsung1y4gb",
		"MarikoHoagSamsung1y8gbX",
		"MarikoAulaSamsung1y4gbX",
		"MarikoIowaMicron1y4gb",
		"MarikoHoagMicron1y4gb",
		"MarikoAulaMicron1y4gb",
		"MarikoAulaSamsung1y8gbX",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_DramId, &id)))
		printf("splGetConfig(SplConfigItem_DramId) failed: 0x%x.\n\n", ret);

	if (id >= 30)
		return dram_desc[30];

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
	snprintf(buf, 19, "%u.%u.%u-%u%u", ver.major, ver.minor, ver.micro, ver.revision_major, ver.revision_minor);

	return buf;
}

char *SwitchIdent_GetHardwareType(void) {
	Result ret = 0;
	u64 hardware_type;

	char *hardware_string[] = {
		"Icosa",
		"Copper",
		"Hoag",
		"Iowa",
		"Calcio",
		"Aula",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_HardwareType, &hardware_type)))
		printf("splGetConfig(SplConfigItem_HardwareType) failed: 0x%x.\n\n", ret);

	if (hardware_type >= 6)
		return hardware_string[6];
	
	return hardware_string[hardware_type];
}

// [4.0.0+] Kiosk mode (0 = retail; 1 = kiosk)
bool SwitchIdent_IsKiosk(void) {
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
	u64 isRetail = 0;

	char *unit[] = {
		"Debug",
		"Retail",
		"Unknown"
	};

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRetail, &isRetail))) {
		printf("splGetConfig(SplConfigItem_IsRetail) failed: 0x%x.\n\n", ret);
		return unit[2];
	}

	return unit[isRetail];
}

bool SwitchIdent_IsSafeMode(void) {
	Result ret = 0;
	u64 safemode = 0;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_IsRecoveryBoot, &safemode)))
		printf("splGetConfig(SplConfigItem_IsRecoveryBoot) failed: 0x%x.\n\n", ret);

	if (safemode)
		return true;

	return false;
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
	static SetSysSerialNumber serial;

	if (R_FAILED(ret = setsysGetSerialNumber(&serial)))
		printf("setsysGetSerialNumber() failed: 0x%x.\n\n", ret);

	return serial.number;
}
