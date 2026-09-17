# Building and Flashing from Visual Studio Code

## 1. Overview

This document provides instructions to build and flash the firmware without the MPLAB X IDE.
You can complete all operations directly inside Visual Studio Code or Windows PowerShell.

---

## 2. Prerequisites

Make sure the following tools are installed on your computer:
1. **Visual Studio Code**
2. **Microchip XC-DSC or XC16 Compiler**
3. **Microchip MPLAB IPE (Integrated Programming Environment)** (installed with MPLAB X)
4. **Hardware Programmer** (PICkit 4, PICkit 5, or MPLAB Snap)

---

## 3. Flash Using VS Code Tasks

The workspace includes configured build tasks in `.vscode/tasks.json`.

### Instructions:
1. Connect your programmer (PICkit 4) to your computer with a USB cable.
2. Connect the programmer 8-pin connector to the motor controller programming header.
3. Apply power to the motor controller board.
4. In VS Code, press `Ctrl + Shift + P`.
5. Type `Tasks: Run Task` and press `Enter`.
6. Select one of the following tasks:
   - `Flash dsPIC33CK (PICkit 4)`
   - `Flash dsPIC33CK (PICkit 5)`
   - `Flash dsPIC33CK (MPLAB Snap)`
7. The terminal executes `ipecmd.exe` and flashes the target microcontroller in 3 to 5 seconds.

---

## 4. Flash Using PowerShell Command Line

You can run the programming command directly in the VS Code terminal.

### Command for PICkit 4:
```powershell
& "C:\Program Files\Microchip\MPLABX\v6.25\mplab_platform\mplab_ipe\ipecmd.exe" `
    -P33CK256MP505 `
    -TPPK4 `
    -M `
    -F"dist\default\production\dspic33ck256mp505_ebike_1kW_v1.1d.X.production.hex"
```

### Parameter Explanations:
- `-P33CK256MP505`: Sets the target microcontroller part number.
- `-TPPK4`: Selects the PICkit 4 programmer tool (`-TPPK5` for PICkit 5, `-TPSNAP` for Snap).
- `-M`: Commands the programmer to write and verify all program memory regions.
- `-F`: Specifies the target `.hex` file path.

---

## 5. Automatic Programming with Standalone MPLAB IPE

If you prefer a graphic tool instead of the command line:

1. Launch **MPLAB IPE** from the Windows Start menu.
2. Set **Device** to `dsPIC33CK256MP505` and click **Apply**.
3. Set **Tool** to `PICkit 4` (or your connected tool) and click **Connect**.
4. Click **Browse** and select the production file:
   `dist\default\production\dspic33ck256mp505_ebike_1kW_v1.1d.X.production.hex`
5. Click **Settings** -> **Advanced Mode** (default password is `microchip`).
6. Enable the option: **Auto Program on Hex File Change**.
7. Now, each time you compile code in VS Code, MPLAB IPE flashes the chip automatically.
