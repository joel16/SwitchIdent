#include <stdio.h>
#include <string.h>

#include <switch.h>

#include "misc.h"
#include "setsys.h"
#include "system.h"

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
		"Unknown"
	};

	SetRegion regionCode = 0;

	if (R_SUCCEEDED(ret = setGetRegionCode(&regionCode))) {
		if (regionCode < 4)
			return regions[regionCode];
	}
	
	printf("setGetRegionCode() failed: 0x%x.\n\n", ret);
	return regions[4];
}

static u32 SwitchIdent_GetPerformanceConfig(void) {
	Result ret = 0;
	u32 performanceConfig = 0;

	if (R_FAILED(ret = apmGetPerformanceConfiguration(appletGetOperationMode(), &performanceConfig)))
		printf("apmGetPerformanceConfiguration() failed: 0x%x.\n\n", ret);

	return performanceConfig;
}

u32 SwitchIdent_GetCPUClock(void) {
	u32 out = 0;

	switch (SwitchIdent_GetPerformanceConfig()) {
		case 0x00010000:
			out = 1020;
			break;
		case 0x00010001:
			out = 1020;
			break;
		case 0x00010002:
			out = 1224;
			break;
		case 0x00020000:
			out = 1020;
			break;
		case 0x00020001:
			out = 1020;
			break;
		case 0x00020002:
			out = 1224;
			break;
		case 0x00020003:
			out = 1020;
			break;
		case 0x00020004:
			out = 1020;
			break;
		case 0x00020005:
			out = 1020;
			break;
		case 0x00020006:
			out = 1020;
			break;
		case 0x92220007:
			out = 1020;
			break;
		case 0x92220008:
			out = 1020;
			break;
	}

	return out;
}

u32 SwitchIdent_GetGPUClock(void) {
	u32 out = 0;

	switch (SwitchIdent_GetPerformanceConfig()) {
		case 0x00010000:
			out = 384;
			break;
		case 0x00010001:
			out = 768;
			break;
		case 0x00010002:
			out = 691.2;
			break;
		case 0x00020000:
			out = 230.4;
			break;
		case 0x00020001:
			out = 307.2;
			break;
		case 0x00020002:
			out = 230.4;
			break;
		case 0x00020003:
			out = 307;
			break;
		case 0x00020004:
			out = 384;
			break;
		case 0x00020005:
			out = 307.2;
			break;
		case 0x00020006:
			out = 384;
			break;
		case 0x92220007:
			out = 460.8;
			break;
		case 0x92220008:
			out = 460.8;
			break;
	}

	return out;
}

char *SwitchIdent_GetBluetoothBdAddress(Service *srv) {
	Result ret = 0;
	static char bd_addr[0x7];

	if (R_FAILED(ret = setcalGetBluetoothBdAddress(srv, bd_addr))) {
		printf("setcalGetBluetoothBdAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	return bd_addr;
}

char *SwitchIdent_GetWirelessLanMacAddress(Service *srv) {
	Result ret = 0;
	static char mac_addr[0x7];

	if (R_FAILED(ret = setcalGetWirelessLanMacAddress(srv, mac_addr))) {
		printf("setcalGetWirelessLanMacAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	return mac_addr;
}
