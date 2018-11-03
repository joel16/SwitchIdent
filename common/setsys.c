#include <string.h>
#include <switch.h>

Result setcalGetBluetoothBdAddress(Service *srv, char *address) {
    IpcCommand c;
    ipcInitialize(&c);

    if (address) memset(address, 0, 0x7);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 0;

    Result rc = serviceIpcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            char address[0x6];
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc) && address)
            memcpy(address, resp->address, 0x6);
    }

    return rc;
}

Result setcalGetWirelessLanMacAddress(Service *srv, char *address) {
    IpcCommand c;
    ipcInitialize(&c);

    if (address) memset(address, 0, 0x7);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 6;

    Result rc = serviceIpcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            char address[0x6];
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc) && address)
            memcpy(address, resp->address, 0x6);
    }

    return rc;
}
