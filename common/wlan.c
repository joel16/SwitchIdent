#include <switch.h>

static Result wlaninfGetState(Service *srv, u32 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 10;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u32 state;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->state;
        }
    }
    
    return rc;
}

static Result wlaninfGetRssi(Service *srv, u32 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 12;
    
    Result rc = serviceIpcDispatch(srv);
    
    if(R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);
        
        struct {
            u64 magic;
            u64 result;
            u32 rssi;
        } *resp = r.Raw;
        
        rc = resp->result;
        
        if (R_SUCCEEDED(rc)) {
            *out = resp->rssi;
        }
    }
    
    return rc;
}

u32 SwitchIdent_GetWlanState(Service *srv) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = wlaninfGetState(srv, &out)))
		return -1;

	return out;
}

u32 SwitchIdent_GetWlanRSSI(Service *srv) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = wlaninfGetRssi(srv, &out)))
		return -1;

	return out;
}