#include <switch.h>

u32 SwitchIdent_GetWlanState(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = wlaninfGetState(&out)))
		return -1;

	return out;
}

u32 SwitchIdent_GetWlanQuality(u32 dBm) {
    u32 quality = 0;

    if (dBm <= -100)
        quality = 0;
    else if (dBm >= -50)
        quality = 100;
    else
        quality = 2 * (dBm + 100);

    return quality;
}

u32 SwitchIdent_GetWlanRSSI(void) {
	Result ret = 0;
	s32 out = 0;

	if (R_FAILED(ret = wlaninfGetRSSI(&out)))
		return -1;

	return out;
}
