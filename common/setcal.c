#include <stdio.h>
#include <string.h>
#include <switch.h>

static Service setcal_service;

Result setcalInitialize(void) {
    return smGetService(&setcal_service, "set:cal");
}

void setcalExit(void) {
    serviceClose(&setcal_service);
}

Result setcalGetBluetoothBdAddress(char *address) {
    IpcCommand c;
    ipcInitialize(&c);

    if (address) memset(address, 0, 0x13);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 0;

    Result rc = serviceIpcDispatch(&setcal_service);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            char address[0x6];
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc) && address) {
            snprintf(address, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", *resp->address, *(resp->address + 0x1), *(resp->address + 0x2), *(resp->address + 0x3), 
                *(resp->address + 0x4), *(resp->address + 0x5));
        }
    }

    return rc;
}

Result setcalGetWirelessLanMacAddress(char *address) {
    IpcCommand c;
    ipcInitialize(&c);

    if (address) memset(address, 0, 0x13);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 6;

    Result rc = serviceIpcDispatch(&setcal_service);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            char address[0x6];
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc) && address) {
            snprintf(address, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", *resp->address, *(resp->address + 0x1), *(resp->address + 0x2), *(resp->address + 0x3), 
                *(resp->address + 0x4), *(resp->address + 0x5));
        }
    }

    return rc;
}

Result setcalGetBatteryLot(char *out) {
    IpcCommand c;
    ipcInitialize(&c);

    if (out) memset(out, 0, 0x19);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 12;

    Result rc = serviceIpcDispatch(&setcal_service);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            char battery_lot[0x18];
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc) && out)
            memcpy(out, resp->battery_lot, 0x18);
    }

    return rc;
}
