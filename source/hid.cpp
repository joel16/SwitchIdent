#include <cstdio>

#include "common.hpp"

namespace SwitchIdent {
    static Result hiddbgGetFirmwareVersion(HidNpadIdType id, u8 deviceType, HIDFirmwareVersion *version) {
        const struct {
            HidNpadIdType a;
            u8 b;
        } in = { id, deviceType };
        
        return serviceDispatchInOut(hiddbgGetServiceSession(), 205, in, *version);
    }

    Result GetJoyconFirmwareVersion(HidDeviceTypeBits deviceType, HIDFirmwareVersion *version) {
        Result ret = 0;

        if (R_FAILED(ret = hiddbgGetFirmwareVersion(HidNpadIdType_No1, deviceType, version))) {
            std::printf("hiddbgGetFirmwareVersion() failed: 0x%x.\n\n", ret);
        }

        return ret;
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

    static Result hidsysGetTouchScreenFirmwareVersion(HIDFirmwareVersion *version) {
        return serviceDispatchOut(hidsysGetServiceSession(), 1151, *version);
    }

    Result GetTouchScreenFirmwareVersion(HIDFirmwareVersion *version) {
        Result ret = 0;

        if (R_FAILED(ret = hidsysGetTouchScreenFirmwareVersion(version))) {
            std::printf("hidsysGetTouchScreenFirmwareVersion() failed: 0x%x.\n\n", ret);
        }

        return ret;
    }
}
