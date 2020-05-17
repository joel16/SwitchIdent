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
	ClkrstSession clkSession;
	Result ret = 0;
	u32 out = 0;
	
	clkrstOpenSession(&clkSession, PcvModuleId_CpuBus, 3);

	if (R_FAILED(ret = clkrstGetClockRate(&clkSession, &out)))
		printf("clkrstGetClockRate(PcvModuleId_CpuBus) failed: 0x%x.\n\n", ret);

	clkrstCloseSession(&clkSession);

	return out/1000000;
}

u32 SwitchIdent_GetGPUClock(void) {
	ClkrstSession clkSession;
	Result ret = 0;
	u32 out = 0;

	clkrstOpenSession(&clkSession, PcvModuleId_GPU, 3);

	if (R_FAILED(ret = clkrstGetClockRate(&clkSession, &out)))
		printf("clkrstGetClockRate(PcvModuleId_GPU) failed: 0x%x.\n\n", ret);

	clkrstCloseSession(&clkSession);
	
	return out/1000000;
}

u32 SwitchIdent_GetEMCClock(void) {
	ClkrstSession clkSession;
	Result ret = 0;
	u32 out = 0;

	clkrstOpenSession(&clkSession, PcvModuleId_EMC, 3);

	if (R_FAILED(ret = clkrstGetClockRate(&clkSession, &out)))
		printf("clkrstGetClockRate(PcvModuleId_EMC) failed: 0x%x.\n\n", ret);

	clkrstCloseSession(&clkSession);
	
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

	if (R_FAILED(ret = setcalGetWirelessLanAddress(mac_addr))) {
		printf("setcalGetWirelessLanMacAddress() failed: 0x%x.\n\n", ret);
		return NULL;
	}

	return mac_addr;
}
