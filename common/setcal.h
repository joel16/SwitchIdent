#ifndef _SWITCHIDENT_SETCAL_H_
#define _SWITCHIDENT_SETCAL_H_

Result setcalInitialize(void);
void setcalExit(void);
Result setcalGetBluetoothBdAddress(char *address);
Result setcalGetWirelessLanMacAddress(char *address);
Result setcalGetBatteryLot(char *out);

#endif
