#include <cstdio>

#include "common.hpp"

namespace SwitchIdent {
    // TODO: Fix this
    static Result hiddbgGetFirmwareVersion(HidsysUniquePadId unique_pad_id, u128 *out) {
        if (hosversionBefore(6,0,0))
            return MAKERESULT(Module_Libnx, LibnxError_IncompatSysVer);

        u128 temp = 0;
        Result ret = serviceDispatchInOut(hiddbgGetServiceSession(), 205, unique_pad_id.id, temp);
        if (R_SUCCEEDED(ret) && out) {
            *out = temp;
        }
        else
            std::printf("serviceDispatchInOut failed: 0x%x\n", ret);
        
        return ret;
    }

    u128 GetJoyconFirmwareVersion(HidsysUniquePadId unique_pad_id) {
        Result ret = 0;
        u128 version = 0;

        if (R_FAILED(ret = hiddbgGetFirmwareVersion(unique_pad_id, &version)))
            std::printf("hiddbgGetFirmwareVersion() failed: 0x%x.\n\n", ret);

        return version;
    }

    HidPowerInfo GetJoyconPowerInfo(HidNpadIdType id) {
        HidPowerInfo info;
        hidGetNpadPowerInfoSingle(id, &info);
        return info;
    }

    HidPowerInfo GetJoyconPowerInfoL(HidNpadIdType id) {
        HidPowerInfo info_left;
        HidPowerInfo info_right;
        hidGetNpadPowerInfoSplit(id, &info_left, &info_right);
        return info_left;
    }

    HidPowerInfo GetJoyconPowerInfoR(HidNpadIdType id) {
        HidPowerInfo info_left;
        HidPowerInfo info_right;
        hidGetNpadPowerInfoSplit(id, &info_left, &info_right);
        return info_right;
    }
}
