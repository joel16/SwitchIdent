#include <stdio.h>
#include <string.h>

#include <switch.h>

#include "misc.h"
#include "setcal.h"

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

	if (R_FAILED(ret = setGetRegionCode(&regionCode))) {
		printf("setGetRegionCode() failed: 0x%x.\n\n", ret);
		return regions[4];
	}

	return regions[regionCode];
}

u32 SwitchIdent_GetCPUClock(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = pcvGetClockRate(PcvModule_CpuBus, &out)))
		printf("pcvGetClockRate(PcvModule_CpuBus) failed: 0x%x.\n\n", ret);

	return out/1000000;
}

u32 SwitchIdent_GetGPUClock(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = pcvGetClockRate(PcvModule_GPU, &out)))
		printf("pcvGetClockRate(PcvModule_GPU) failed: 0x%x.\n\n", ret);
	
	return out/1000000;
}

u32 SwitchIdent_GetEMCClock(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = pcvGetClockRate(PcvModule_EMC , &out)))
		printf("pcvGetClockRate(PcvModule_EMC ) failed: 0x%x.\n\n", ret);
	
	return out/1000000;
}

char *SwitchIdent_GetBluetoothBdAddress(void) {
	Result ret = 0;
	static char bd_addr[0x13];

	if (R_FAILED(ret = setcalGetBluetoothBdAddress(bd_addr))) {
		printf("setcalGetBluetoothBdAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	return bd_addr;
}

char *SwitchIdent_GetWirelessLanMacAddress(void) {
	Result ret = 0;
	static char mac_addr[0x13];

	if (R_FAILED(ret = setcalGetWirelessLanMacAddress(mac_addr))) {
		printf("setcalGetWirelessLanMacAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	return mac_addr;
}
