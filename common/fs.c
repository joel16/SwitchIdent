#include <stdio.h>
#include "fs.h"

static Result fsDeviceOperatorIsGameCardInserted(FsDeviceOperator *d, bool *out) {
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 200;

    Result rc = serviceIpcDispatch(&d->s);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            u8 is_inserted;
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc)) {
            *out = resp->is_inserted != 0;
        }
    }

    return rc;
}

bool SwitchIdent_IsSDCardInserted(FsDeviceOperator *fsDeviceOperator) {
	bool inserted;
    Result ret = 0;

	if (R_FAILED(ret = fsDeviceOperatorIsSdCardInserted(fsDeviceOperator, &inserted))) {
        printf("fsDeviceOperatorIsSdCardInserted() failed: 0x%x.\n\n", ret);
		return false;
    }

	return inserted;
}

bool SwitchIdent_IsGameCardInserted(FsDeviceOperator *fsDeviceOperator) {
	bool inserted;
    Result ret = 0;

	if (R_FAILED(ret = fsDeviceOperatorIsGameCardInserted(fsDeviceOperator, &inserted))) {
        printf("fsDeviceOperatorIsGameCardInserted() failed: 0x%x.\n\n", ret);
		return false;
    }

	return inserted;
}
