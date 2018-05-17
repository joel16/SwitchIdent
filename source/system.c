#include <stdio.h>
#include <string.h>

#include <switch.h>

#include "system.h"

const char *SwitchIdent_GetLanguage(void)
{
	Result ret = 0;
	u64 language = 0;
	
	if (R_FAILED(ret = setGetSystemLanguage(&language)))
		printf("setGetSystemLanguage(language) failed: 0x%x.\n\n", ret);
	
	return strupr((char*)&language);
}

// Not sure about this yet.
const char *SwitchIdent_GetRegion(void)
{
	Result ret = 0;

	const char *regions[]=
	{
		"JPN",
		"USA",
		"EUR",
		"AUS",
		"CHN",
		"KOR",
		"TWN",
		"Unknown"
	};

	s32 regionCode = 0;

	if (R_SUCCEEDED(setGetRegionCode(&regionCode)))
	{
		if (regionCode < 7)
			return regions[regionCode];
	}
	
	printf("setGetRegionCode(regionCode) failed: 0x%x.\n\n", ret);
	return regions[7];
}