#include <stdio.h>

#include <switch.h>

#include "system.h"

const char *SwitchIdent_GetLanguage(void)
{
	Result ret = 0;

	const char *languages[] = 
	{
		"Japanese",
		"US English",
		"French",
		"German",
		"Italian",
		"Spanish",
		"Simplified Chinese",
		"Korean",
		"Dutch",
		"Portugese",
		"Russian",
		"Traditional Chinese",
		"UK English",
		"Canadian French",
		"Latin American Spanish",
		"Unknown"
	};

	u64 language = 0;
	
	if (R_SUCCEEDED(ret = setGetSystemLanguage(&language)))
	{
		if (language < 15)
			return languages[language];
	}
	
	printf("setGetSystemLanguage(language) failed: 0x%x.\n\n", ret);
	return languages[15];
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