#include <string.h>
#include <switch.h>

#include "setsys.h"

static Result GetFirmwareVersion(Service *srv, SetSysFirmwareVersion *ver)
{
    char buffer[0x100];
    size_t size = sizeof(buffer);
    memset(buffer, 0, size);

    IpcCommand c;
    ipcInitialize(&c);
    ipcAddRecvStatic(&c, buffer, size, 0);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 3;

    Result rc = serviceIpcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        }* resp = r.Raw;
        rc = resp->result;

        if (R_SUCCEEDED(rc)){
            memcpy(ver, &buffer, sizeof(buffer));
        }
    }

    return rc;
}

static Result GetFirmwareVersion2(Service *srv, SetSysFirmwareVersion *ver)
{
    char buffer[0x100];
    size_t size = sizeof(buffer);
    memset(buffer, 0, size);

    IpcCommand c;
    ipcInitialize(&c);
    ipcAddRecvStatic(&c, buffer, size, 0);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 4;

    Result rc = serviceIpcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        }* resp = r.Raw;
        rc = resp->result;

        if (R_SUCCEEDED(rc)){
            memcpy(ver, &buffer, sizeof(buffer));
        }
    }

    return rc;
}

Result setsysGetFirmwareVersion(Service *srv, SetSysFirmwareVersion *ver)
{
	if (kernelAbove200())
		return GetFirmwareVersion2(srv, ver);
	return GetFirmwareVersion(srv, ver);
}

Result setcalGetBluetoothBdAddress(Service *srv, char *address)
{
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

Result setcalGetWirelessLanMacAddress(Service *srv, char *address)
{
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
