#include <cstdio>

#include "common.hpp"
#include "gui.hpp"
#include "menus.hpp"

namespace Services {
    void Exit(void) {
        // hiddbgExit();
        // hidsysExit();
        wlaninfExit();
        
        if (hosversionAtLeast(8, 0, 0))
            clkrstExit();
        else
            pcvExit();
            
        psmExit();
        nsExit();
        apmExit();
        appletExit();
        socketExit();
        nifmExit();
        splExit();
        setcalExit();
        setsysExit();
        setExit();
        GUI::Exit();
        romfsExit();
        socketExit();
    }

    void Init(void) {
        Result ret = 0;

        socketInitializeDefault();
        nxlinkStdio();
        
        if (R_FAILED(ret = romfsInit()))
            std::printf("romfsInit() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = setInitialize()))
            std::printf("setInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = setsysInitialize()))
            std::printf("setsysInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = setcalInitialize()))
            std::printf("setcalInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = splInitialize()))
            std::printf("splInitialize() failed: 0x%x.\n\n", ret);
        
        if (R_FAILED(ret = nifmInitialize(NifmServiceType_User)))
            std::printf("nifmInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = socketInitializeDefault()))
            std::printf("socketInitializeDefault() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = appletInitialize()))
            std::printf("appletInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = apmInitialize()))
            std::printf("apmInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = nsInitialize()))
            std::printf("nsInitialize() failed: 0x%x.\n\n", ret);
            
        if (R_FAILED(ret = psmInitialize()))
            std::printf("psmInitialize() failed: 0x%x.\n\n", ret);
            
        if (hosversionAtLeast(8, 0, 0)) {
            if (R_FAILED(ret = clkrstInitialize()))
                std::printf("clkrstInitialize() failed: 0x%x.\n\n", ret);
        } else {
            if (R_FAILED(ret = pcvInitialize()))
                std::printf("pcvInitialize() failed: 0x%x.\n\n", ret);
        }
        
        if (hosversionBefore(15, 0, 0)) {
            if (R_FAILED(ret = wlaninfInitialize()))
                std::printf("wlaninfInitialize() failed: 0x%x.\n\n", ret);
        }

        // if (R_FAILED(ret = hidsysInitialize()))
        //     std::printf("hidsysInitialize() failed: 0x%x.\n\n", ret);

        // if (R_FAILED(ret = hiddbgInitialize()))
        //     std::printf("hiddbgInitialize() failed: 0x%x.\n\n", ret);
            
        GUI::Init();
    }
}

int main(int argc, char **argv) {
    Services::Init();
    Menus::Main();
    Services::Exit();
}
