# dsPIC33CK 250W Motor Controller Firmware
### Shell Eco-marathon Battery Electric Prototype (54V Nominal)

## 1. Overview

This repository contains motor control firmware for the Microchip dsPIC33CK256MP505 digital signal controller.
The firmware controls a 250 W permanent magnet synchronous motor in a Shell Eco-marathon Battery Electric vehicle.

The system uses Field-Oriented Control (FOC) with space-vector modulation to deliver maximum electrical efficiency.

---

## 2. Key Features

- **Field-Oriented Control (FOC)**: Fast 20 kHz current control loop with SVPWM generation.
- **Shell Eco-marathon Safe-Start**: Zero-throttle power-on interlock prevents motor startup until the throttle rests at zero.
- **54 V Battery Protection**: Hardware overvoltage cutoff (62.6 V) and undervoltage cutoff (43.2 V).
- **250 W Efficiency Limits**: Continuous current limit of 4.6 A and peak acceleration limit of 7.0 A to minimize $I^2R$ copper losses.
- **Burn-and-Coast Freewheeling**: Inverter gate signals disable when the throttle is released to enable drag-free coasting.
- **Real-Time Telemetry**: 10 Hz serial stream over UART1 (115200 baud) for live potentiometer and speed monitoring.
- **VS Code Integration**: Preconfigured tasks in `.vscode/tasks.json` to flash the target without the MPLAB X IDE.

---

## 3. Directory Structure

```
├── docs/                                  # Technical documentation (ASD-STE100)
│   ├── SYSTEM_DESCRIPTION.md              # System specs, pinout, and state machine
│   ├── THROTTLE_CALIBRATION_AND_TELEMETRY.md # Potentiometer wiring, calibration, & serial monitor
│   ├── SHELL_ECO_MARATHON_OPTIMIZATION.md # 250W tuning, Burn & Coast strategy, & scrutineering
│   └── VSCODE_BUILD_AND_FLASH.md          # Flashing instructions via VS Code and ipecmd
├── hal/                                   # Hardware abstraction layer
│   ├── board_service.c                    # Telemetry stream, button service, PWM overrides
│   └── measure.c                          # Current and voltage measurement routines
├── src/                                   # Motor drive core source files
│   ├── control.h                          # FOC control structures
│   ├── fault.c / fault.h                  # 54V voltage and current fault protection
│   ├── motor_drive.c                      # FOC forward/feedback paths, state machine, throttle
│   └── userparms.h                        # Motor specs, 250W parameters, PI gains, deadbands
├── mcc_generated_files/                   # Microchip Code Configurator peripheral drivers
├── .vscode/                               # VS Code configuration
│   └── tasks.json                         # Flashing tasks for PICkit 4, PICkit 5, and Snap
├── main.c                                 # Application entry point and safety interlock
└── README.md                              # This file
```

---

## 4. Quick Start

### Hardware Setup:
1. Connect potentiometer wiper to pin `RC6` (`AN17`).
2. Connect potentiometer power terminals to `+3.3V` and `GND`.
3. Connect a 10 kOhm pull-down resistor between `RC6` and `GND`.
4. Connect a USB-to-UART serial cable:
   - USB `RX` -> dsPIC `RC8` (TX)
   - USB `GND` -> dsPIC `GND`

### Monitor Telemetry:
Open a serial terminal at **115200 baud, 8-N-1** on the USB-to-UART COM port:
```
POT:12400 V_mV:1248 THR%:36 RUN:1 STATE:2
```

### Flash Microcontroller from VS Code:
1. Connect your PICkit 4 / PICkit 5 / Snap programmer.
2. Press `Ctrl + Shift + P` -> `Tasks: Run Task` -> `Flash dsPIC33CK (PICkit 4)`.

---

## 5. Documentation Links

For detailed instructions, refer to the documentation in the [`docs/`](file:///c:/Users/jgabt/Documents/c1/firmware/dspic33ck256mp505_ebike_1kW_v1.1d.X/docs/) folder:
- [System Description](file:///c:/Users/jgabt/Documents/c1/firmware/dspic33ck256mp505_ebike_1kW_v1.1d.X/docs/SYSTEM_DESCRIPTION.md)
- [Throttle Calibration & Telemetry](file:///c:/Users/jgabt/Documents/c1/firmware/dspic33ck256mp505_ebike_1kW_v1.1d.X/docs/THROTTLE_CALIBRATION_AND_TELEMETRY.md)
- [Shell Eco-Marathon Optimization Guide](file:///c:/Users/jgabt/Documents/c1/firmware/dspic33ck256mp505_ebike_1kW_v1.1d.X/docs/SHELL_ECO_MARATHON_OPTIMIZATION.md)
- [VS Code Build and Flash Guide](file:///c:/Users/jgabt/Documents/c1/firmware/dspic33ck256mp505_ebike_1kW_v1.1d.X/docs/VSCODE_BUILD_AND_FLASH.md)
