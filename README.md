# SwitchIdent ![Github latest downloads](https://img.shields.io/github/downloads/joel16/SwitchIdent/total.svg)

This is yet another identity tool that is continuing the series of <*device name here*>ident. This tool allows users to get various bits of information from your Nintendo Switch device, hence the name 'ident' as in identifying your Nintendo Switch.

<p align="center">
<img src="https://i.imgur.com/JfPrael.jpg" alt="SwitchIdent Screenshot" width="640" height="360"/>
</p>

# Features: 
- Displays system firmware version.
- Displays hardware type and unit.
- Displays serial number.
- Displays DRAM ID.
- Displays device ID.
- Displays system region.
- Displays CPU, GPU and EMC clocks.
- Displays whether the switch is in docked/handheld state.
- Displays IP Address.
- Displays storage sizes for SD, NAND (user) and NAND (system).
- Displays various system statuses. (Bluetooth, WLAN, NFC, etc.)
- Displays battery percentage and charging status.
- Displays if battery charging is enabled.
- Displays if ample power is supplied.
- Displays wireless RSSI signals and quality.
- Displays battery voltage sate.
- Displays battery lot number.
- Displays SD and gamecard slot statuses.
- Displays WiFi and Bluetooth MAC address.
- Display battery age (in percentage and pcm).
- Display internal (PCB) temperature.
- Display external (SoC) temperature.
- Display battery charger input and output current limit.
- Display battery charging current limit.
- Display battery charging voltage limit.
- Display battery capacity (pcm) and age (pcm).
- Display battery voltage avg.
- Display battery charger type.
- Display battery charger current limit.
- Display battery charger voltage limit.
- Display battery temperature.
- Display headphone port status.
- Display joy-con battery.
- Display joy-con firmware.

# Credits:
- shchmue: Added missing DRAM descriptors and fix compatibility with libnx v4.2.0+
- Eve/Hikari/Junko, Klodeckel and hopperplaysmc: Beta testing
- preetisketch: Banner
- masagrator for pointing out that the internal/external temperatures were related to PCB and SoC respectively.
- SwitchBrew wiki for documenting various service API uses in SwitchIdent.
- devkitPro for the devkitA64 toolchain and libnx support.
