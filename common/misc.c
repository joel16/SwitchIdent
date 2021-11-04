#include <stdio.h>
#include <switch.h>

char *SwitchIdent_GetOperationMode(void) {
	if (appletGetOperationMode() == AppletOperationMode_Handheld)
		return "Handheld";

	return "Docked";
}

bool SwitchIdent_GetWirelessLanEnableFlag() {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = setsysGetWirelessLanEnableFlag(&out)))
		printf("setsysGetWirelessLanEnableFlag() failed: 0x%x.\n\n", ret);

	return out;
}

bool SwitchIdent_GetBluetoothEnableFlag() {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = setsysGetBluetoothEnableFlag(&out)))
		printf("setsysGetBluetoothEnableFlag() failed: 0x%x.\n\n", ret);

	return out;
}

bool SwitchIdent_GetNfcEnableFlag() {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = setsysGetNfcEnableFlag(&out)))
		printf("setsysGetNfcEnableFlag() failed: 0x%x.\n\n", ret);

	return out;
}

bool SwitchIdent_GetAutoUpdateEnableFlag() {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = setsysGetAutoUpdateEnableFlag(&out)))
		printf("setsysGetAutoUpdateEnableFlag() failed: 0x%x.\n\n", ret);

	return out;
}

bool SwitchIdent_GetConsoleInformationUploadFlag() {
	Result ret = 0;
	bool out = false;

	if (R_FAILED(ret = setsysGetConsoleInformationUploadFlag(&out)))
		printf("setsysGetConsoleInformationUploadFlag() failed: 0x%x.\n\n", ret);

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
