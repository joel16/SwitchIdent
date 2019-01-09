#include <switch.h>

static Service wlaninf_service;

Result wlaninfInitialize(void) {
    return smGetService(&wlaninf_service, "wlan:inf");
}

void wlaninfExit(void) {
    serviceClose(&wlaninf_service); 
}

static Result wlaninfGetState(u32 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 10;
    
    Result rc = serviceIpcDispatch(&wlaninf_service);
    
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

static Result wlaninfGetRssi(u32 *out) {
    IpcCommand c;
    ipcInitialize(&c);
    
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;
    
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 12;
    
    Result rc = serviceIpcDispatch(&wlaninf_service);
    
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

u32 SwitchIdent_GetWlanState(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = wlaninfGetState(&out)))
		return -1;

	return out;
}

u32 SwitchIdent_GetWlanQuality(u32 dBm) {
    u32 quality = 0;

    if (dBm <= -100)
        quality = 0;
    else if (dBm >= -50)
        quality = 100;
    else
        quality = 2 * (dBm + 100);

    return quality;
}

u32 SwitchIdent_GetWlanRSSI(void) {
	Result ret = 0;
	u32 out = 0;

	if (R_FAILED(ret = wlaninfGetRssi(&out)))
		return -1;

	return out;
}
