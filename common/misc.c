#include <stdio.h>
#include <switch.h>

char *SwitchIdent_GetOperationMode(void)
{
	if (appletGetOperationMode() == AppletOperationMode_Handheld)
		return "Handheld";

	return "Docked";
}

bool SwitchIdent_GetWLANStatus(void)
{
	Result ret = 0;
	bool wlan_flag = 0;

	if (R_FAILED(ret = setsysGetWirelessLanEnableFlag(&wlan_flag)))
		printf("setsysGetWirelessLanEnableFlag() failed: 0x%x.\n\n", ret);

	return wlan_flag;
}

bool SwitchIdent_GetBluetoothStatus(void)
{
	Result ret = 0;
	bool bluetooth_flag = 0;

	if (R_FAILED(ret = setsysGetBluetoothEnableFlag(&bluetooth_flag)))
		printf("setsysGetBluetoothEnableFlag() failed: 0x%x.\n\n", ret);

	return bluetooth_flag;
}

bool SwitchIdent_GetNFCStatus(void)
{
	Result ret = 0;
	bool nfc_flag = 0;

	if (R_FAILED(ret = setsysGetNfcEnableFlag(&nfc_flag)))
		printf("setsysGetNfcEnableFlag() failed: 0x%x.\n\n", ret);

	return nfc_flag;
}

bool SwitchIdent_GetLockscreenStatus(void)
{
	Result ret = 0;
	bool lockscreen_flag = 0;

	if (R_FAILED(ret = setsysGetLockScreenFlag(&lockscreen_flag)))
		printf("setsysGetLockScreenFlag() failed: 0x%x.\n\n", ret);

	return lockscreen_flag;
}

bool SwitchIdent_GetAutoAppDownloadStatus(void)
{
	Result ret = 0;
	bool automaitc_app_dl_flag = 0;

	if (R_FAILED(ret = setsysGetAutomaticApplicationDownloadFlag(&automaitc_app_dl_flag)))
		printf("setsysGetAutomaticApplicationDownloadFlag() failed: 0x%x.\n\n", ret);

	return automaitc_app_dl_flag;
}

bool SwitchIdent_GetConsoleInfoUploadStatus(void)
{
	Result ret = 0;
	bool consoleinfo_upload_flag = 0;

	if (R_FAILED(ret = setsysGetConsoleInformationUploadFlag(&consoleinfo_upload_flag)))
		printf("setsysGetConsoleInformationUploadFlag() failed: 0x%x.\n\n", ret);

	return consoleinfo_upload_flag;
}
