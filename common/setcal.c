#include <stdio.h>
#include <string.h>
#include <switch.h>

Result setcalGetBluetoothBdAddress(char *address) {
    
    SetCalBdAddress addr = {0};

    Result rc = setcalGetBdAddress(&addr);

    if (R_SUCCEEDED(rc) && address) {
        snprintf(address, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", addr.bd_addr[0], addr.bd_addr[1],
            addr.bd_addr[2],addr.bd_addr[3],addr.bd_addr[4],addr.bd_addr[5]);
    }

    return rc;
}

Result setcalGetWirelessLanAddress(char *address) {
    
    SetCalMacAddress addr = {0};

    Result rc = setcalGetWirelessLanMacAddress(&addr);

    if (R_SUCCEEDED(rc) && address) {
        snprintf(address, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", addr.addr[0], addr.addr[1],
            addr.addr[2],addr.addr[3],addr.addr[4],addr.addr[5]);
    }

    return rc;
}
