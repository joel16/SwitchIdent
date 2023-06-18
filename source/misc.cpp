#include <cstdio>

#include "common.hpp"

namespace SwitchIdent {
    const char *GetOperationMode(void) {
        return (appletGetOperationMode() == AppletOperationMode_Handheld)? "Handheld" : "Docked";
    }
    
    bool GetWirelessLanEnableFlag(void) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = setsysGetWirelessLanEnableFlag(&out))) {
            std::printf("setsysGetWirelessLanEnableFlag() failed: 0x%x.\n\n", ret);
        }
        
        return out;
    }
    
    bool GetBluetoothEnableFlag(void) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = setsysGetBluetoothEnableFlag(&out))) {
            std::printf("setsysGetBluetoothEnableFlag() failed: 0x%x.\n\n", ret);
        }
        
        return out;
    }
    
    bool GetNfcEnableFlag(void) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = setsysGetNfcEnableFlag(&out))) {
            std::printf("setsysGetNfcEnableFlag() failed: 0x%x.\n\n", ret);
        }
        
        return out;
    }
    
    bool GetAutoUpdateEnableFlag(void) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = setsysGetAutoUpdateEnableFlag(&out))) {
            std::printf("setsysGetAutoUpdateEnableFlag() failed: 0x%x.\n\n", ret);
        }
        
        return out;
    }
    
    bool GetConsoleInformationUploadFlag(void) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = setsysGetConsoleInformationUploadFlag(&out))) {
            std::printf("setsysGetConsoleInformationUploadFlag() failed: 0x%x.\n\n", ret);
        }
            
        return out;
    }
    
    bool IsSDCardInserted(FsDeviceOperator *fsDeviceOperator) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = fsDeviceOperatorIsSdCardInserted(fsDeviceOperator, &out))) {
            std::printf("fsDeviceOperatorIsSdCardInserted() failed: 0x%x.\n\n", ret);
        }
            
        return out;
    }
    
    bool IsGameCardInserted(FsDeviceOperator *fsDeviceOperator) {
        Result ret = 0;
        bool out = false;
        
        if (R_FAILED(ret = fsDeviceOperatorIsGameCardInserted(fsDeviceOperator, &out))) {
            std::printf("fsDeviceOperatorIsGameCardInserted() failed: 0x%x.\n\n", ret);
        }
            
        return out;
    }
}
