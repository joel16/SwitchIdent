#include <cstdio>
#include "common.hpp"

namespace SwitchIdent {
    s64 GetTotalStorage(NcmStorageId storage_id) {
        Result ret = 0;
        s64 total = 0;
        
        if (R_FAILED(ret = nsGetTotalSpaceSize(storage_id, &total))) {
            std::printf("nsGetFreeSpaceSize() failed: 0x%x.\n\n", ret);
        }
            
        return total;
    }
    
    s64 GetFreeStorage(NcmStorageId storage_id) {
        Result ret = 0;
        s64 free = 0;
        
        if (R_FAILED(ret = nsGetFreeSpaceSize(storage_id, &free))) {
            std::printf("nsGetFreeSpaceSize() failed: 0x%x.\n\n", ret);
        }
            
        return free;
    }
    
    s64 GetUsedStorage(NcmStorageId storage_id) {
        return (SwitchIdent::GetTotalStorage(storage_id) - SwitchIdent::GetFreeStorage(storage_id));
    }
    
    void GetSizeString(char *string, double size) {
        int i = 0;
        const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
        
        while (size >= 1024.0f) {
            size /= 1024.0f;
            i++;
        }
        
        std::sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, size, units[i]);
    }
}
