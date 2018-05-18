#include <switch.h>

const char *SwitchIdent_GetOperationMode(void)
{
	if (appletGetOperationMode() == AppletOperationMode_Handheld)
		return "Handheld";

	return "Docked";
}
