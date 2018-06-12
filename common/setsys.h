#ifndef _SWITCHIDENT_SETSYS_H_
#define _SWITCHIDENT_SETSYS_H_

typedef struct {
    u8 version_raw[8];
    char platform[32];
    char hash[64];
    char version_short[24];
    char version_long[128];
} SetSysFirmwareVersion;

Result setsysGetFirmwareVersion(Service *srv, SetSysFirmwareVersion *ver);

#endif