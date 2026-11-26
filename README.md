# dsPIC33CK Maxon EC 48 V Motor Controller Firmware
### Shell Eco-marathon Battery Electric Prototype (54 V pack)

## 1. Overview

This repository contains motor control firmware for the Microchip dsPIC33CK256MP505 digital signal controller.
The firmware controls a Maxon EC 48 V (part 634043) permanent magnet synchronous motor in a Shell Eco-marathon Battery Electric vehicle.
The motor has 8 pole pairs, 3610 rpm nominal speed, 627 mNm nominal torque, and 5.48 A nominal (maximum continuous) current.

The system uses Field-Oriented Control (FOC) with space-vector modulation to deliver maximum electrical efficiency.

---

## 2. Key Features

- **Field-Oriented Control (FOC)**: Fast 20 kHz current control loop with SVPWM generation, full linear voltage utilization, and conservative high-speed field weakening.
- **Shell Eco-marathon Safe-Start**: Zero-throttle start interlock prevents motor startup until the throttle rests at zero.
- **54 V Battery Protection**: Hardware overvoltage cutoff (63.5 V) and undervoltage cutoff (43.2 V).
- **Maxon EC 48 V Limits**: 5.48 A hard current cap (the motor's nominal current), with no software speed limit.
- **Automatic Soft Start**: Every launch ramps torque at 15 A/s with 50 % current-loop gains for 500 ms, then fades to full gains. Roll-off is also limited to 15 A/s and the throttle is filtered for 20 ms. No rider mode switch.
- **Fixed Hall/Phase Direction**: Uses the configured Hall table and forward direction; phase and Hall wiring must match.
- **Burn-and-Coast Freewheeling**: Inverter gate signals disable when the throttle is released to enable drag-free coasting.
- **Real-Time Telemetry**: 10 Hz serial stream over UART1 (115200 baud) for live potentiometer and speed monitoring.
- **VS Code Integration**: Preconfigured tasks in `.vscode/tasks.json` to flash the target without the MPLAB X IDE.

---

## 3. Directory Structure

```
├── docs/                                  # Technical documentation (ASD-STE100)
│   ├── SYSTEM_DESCRIPTION.md              # System specs, pinout, and state machine
│   ├── THROTTLE_CALIBRATION_AND_TELEMETRY.md # Potentiometer wiring, calibration, & serial monitor
│   ├── TELEMETRY_DATA_DICTIONARY.md          # ESP32 UART field definitions and parsing rules
│   ├── FOC_VALIDATION_PLAN.md              # Staged bench tests and acceptance criteria
│   ├── SHELL_ECO_MARATHON_OPTIMIZATION.md # Maxon 48 V tuning, Burn & Coast strategy, & scrutineering
│   └── VSCODE_BUILD_AND_FLASH.md          # Flashing instructions via VS Code and ipecmd
├── hal/                                   # Hardware abstraction layer
│   ├── board_service.c                    # Telemetry stream, button service, PWM overrides
│   └── measure.c                          # Current and voltage measurement routines
├── src/                                   # Motor drive core source files
│   ├── control.h                          # FOC control structures
│   ├── fault.c / fault.h                  # 54V voltage and current fault protection
│   ├── motor_drive.c                      # FOC forward/feedback paths, state machine, throttle
│   └── userparms.h                        # Maxon EC 48 V specs, PI gains, deadbands
├── mcc_generated_files/                   # Microchip Code Configurator peripheral drivers
├── tools/                                 # Host-side test utilities
│   └── capture_telemetry.ps1               # UART telemetry logger to CSV
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
POT:12400 POT_mV:1248 THR%:36 RUN:1 STATE:2 ... VBUS_mV:54000 IQ_mA:0 ...
```

### Flash Microcontroller from VS Code:
1. Connect your PICkit 4 / PICkit 5 / Snap programmer.
2. Press `Ctrl + Shift + P` -> `Tasks: Run Task` -> `Flash dsPIC33CK (PICkit 4)`.

---

## 5. Documentation Links

For detailed instructions, refer to the documentation in the [`docs/`](docs/) folder:
- [System Description](docs/SYSTEM_DESCRIPTION.md)
- [Throttle Calibration & Telemetry](docs/THROTTLE_CALIBRATION_AND_TELEMETRY.md)
- [Shell Eco-Marathon Optimization Guide](docs/SHELL_ECO_MARATHON_OPTIMIZATION.md)
- [VS Code Build and Flash Guide](docs/VSCODE_BUILD_AND_FLASH.md)
