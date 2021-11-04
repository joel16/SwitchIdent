#include <stdio.h>
#include <string.h>

#include <switch.h>

#include "misc.h"

char *SwitchIdent_GetLanguage(void) {
	Result ret = 0;
	u64 language = 0;
	
	if (R_FAILED(ret = setGetSystemLanguage(&language)))
		printf("setGetSystemLanguage() failed: 0x%x.\n\n", ret);
	
	return strupr((char*)&language);
}

char *SwitchIdent_GetRegion(void) {
	Result ret = 0;

	char *regions[] = {
		"JPN",
		"USA",
		"EUR",
		"AUS",
		"CHN",
		"KOR",
		"TWN",
		"Unknown"
	};

	SetRegion regionCode = 0;

	if (R_FAILED(ret = setGetRegionCode(&regionCode))) {
		printf("setGetRegionCode() failed: 0x%x.\n\n", ret);
		return regions[7];
	}

	return regions[regionCode];
}

u32 SwitchIdent_GetClock(PcvModule module) {
	Result ret = 0;
	u32 out = 0;

	if (hosversionAtLeast(8, 0, 0)) {
		ClkrstSession session = {0};
		PcvModuleId module_id;

		if (R_FAILED(ret = pcvGetModuleId(&module_id, module)))
			printf("pcvGetModuleId() failed: 0x%x.\n\n", ret);
		else if (R_FAILED(ret = clkrstOpenSession(&session, module_id, 3)))
			printf("clkrstOpenSession() failed: 0x%x.\n\n", ret);
		else if (R_FAILED(ret = clkrstGetClockRate(&session, &out)))
			printf("clkrstGetClockRate() failed: 0x%x.\n\n", ret);
		else
			clkrstCloseSession(&session);
	} else {
		if (R_FAILED(ret = pcvGetClockRate(module, &out)))
			printf("pcvGetClockRate() failed: 0x%x.\n\n", ret);
	}

	return out/1000000;
}

char *SwitchIdent_GetBluetoothBdAddress(void) {
	Result ret = 0;
	static char bd_addr_string[0x13];
	SetCalBdAddress bd_addr;

	if (R_FAILED(ret = setcalGetBdAddress(&bd_addr))) {
		printf("setcalGetBdAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	snprintf(bd_addr_string, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", bd_addr.bd_addr[0], bd_addr.bd_addr[1], bd_addr.bd_addr[2], bd_addr.bd_addr[3], bd_addr.bd_addr[4], bd_addr.bd_addr[5]);

	return bd_addr_string;
}

char *SwitchIdent_GetWirelessLanMacAddress(void) {
	Result ret = 0;
	static char mac_addr_string[0x13];
	SetCalMacAddress mac_addr;

	if (R_FAILED(ret = setcalGetWirelessLanMacAddress(&mac_addr))) {
		printf("setcalGetWirelessLanMacAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	snprintf(mac_addr_string, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr.addr[0], mac_addr.addr[1], mac_addr.addr[2], mac_addr.addr[3], mac_addr.addr[4], mac_addr.addr[5]);

	return mac_addr_string;
}
