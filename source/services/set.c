#include <string.h>

#include "set.h"

static Service g_setsysSrv;

Result setsysInitialize2(void)
{
    if (serviceIsActive(&g_setsysSrv))
        return MAKERESULT(Module_Libnx, LibnxError_AlreadyInitialized);

    return smGetService(&g_setsysSrv, "set:sys");
}

void setsysExit2(void)
{
	serviceClose(&g_setsysSrv);
}

Result setGetSerialNumber(char *serial) {
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 68;

    Result rc = serviceIpcDispatch(&g_setsysSrv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            char serial[0x18];
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc) && serial)
        	memcpy(serial, resp->serial, 0x18);
    }

    return rc;
}
