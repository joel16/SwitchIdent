#include <stdio.h>
#include <switch.h>

char *SwitchIdent_GetOperationMode(void) {
	if (appletGetOperationMode() == AppletOperationMode_Handheld)
		return "Handheld";

	return "Docked";
}

bool SwitchIdent_GetFlag(SetSysFlag flag) {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = setsysGetFlag(flag, &out)))
		printf("setsysGetFlag() failed: 0x%x.\n\n", ret);

	return out;
}

bool SwitchIdent_IsSDCardInserted(FsDeviceOperator *fsDeviceOperator) {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = fsDeviceOperatorIsSdCardInserted(fsDeviceOperator, &out)))
		printf("fsDeviceOperatorIsSdCardInserted() failed: 0x%x.\n\n", ret);

	return out;
}

bool SwitchIdent_IsGameCardInserted(FsDeviceOperator *fsDeviceOperator) {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = fsDeviceOperatorIsGameCardInserted(fsDeviceOperator, &out)))
		printf("fsDeviceOperatorIsGameCardInserted() failed: 0x%x.\n\n", ret);

	return out;
}
