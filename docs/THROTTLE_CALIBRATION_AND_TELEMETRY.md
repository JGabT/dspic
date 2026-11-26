# Throttle Calibration and Real-Time Telemetry

## 1. Overview

This document describes the throttle potentiometer interface, safety interlocks, and serial telemetry.
The firmware reads the potentiometer voltage through analog channel AN17 (pin RC6).

---

## 2. Safety Interlock (Safe-Start Protection)

The controller has a zero-throttle start interlock.
This interlock complies with Shell Eco-marathon technical inspection rules.

### Operation:
1. Turn on the 54 V battery power switch.
2. The firmware initializes and checks the potentiometer reading.
3. If the throttle voltage is higher than 0.76 V (7547 counts), the motor does not turn.
4. The driver must release the throttle to (or below) the 0.76 V deadband.
5. When the firmware reads a voltage at or below 0.76 V, the safety interlock arms the motor.
6. The driver can then turn the throttle to accelerate the vehicle.

---

## 3. Throttle Calibration Thresholds

The file `src/userparms.h` defines the throttle calibration thresholds:

```c
#define THROTTLE_LOW        7547   // 0.76 V - lower deadband
#define THROTTLE_HIGH       23500  // 2.37 V - full throttle
#define THROTTLE_HYSTERESIS 200    // 0.02 V - anti-chatter window
```

### Threshold Definitions:
- **THROTTLE_LOW (0.76 V / 7547 counts)**:
  Signals at or below this voltage are in the resting deadband.
  The controller commands zero torque and shuts off inverter gate signals.
- **THROTTLE_HIGH (2.37 V / 23500 counts)**:
  Signals at or above this voltage command 100% of the active torque ceiling.
- **THROTTLE_HYSTERESIS (0.02 V / 200 counts)**:
  Prevents motor chatter when the throttle rests near the deadband edge.
  The motor activates at `0.78 V` (7547 + 200 counts).
  The motor deactivates at `0.76 V` (7547 counts).

---

## 4. Hardware Wiring for Potentiometer

Connect the 3-wire potentiometer to the controller board as follows:

```
[ Controller Board ]                 [ Potentiometer ]
  +3.3V Pin        ----------------> Terminal 1 (VCC)
  RC6 / AN17 Pin   ----------------> Terminal 2 (Wiper)
  GND Pin          ----------------> Terminal 3 (Ground)
```

> [!WARNING]
> Do not connect the potentiometer supply pin to 5 V.
> The microcontroller analog input pins accept a maximum voltage of 3.3 V.
> A 5 V signal can permanently damage the microcontroller pin.

### Fail-Safe Pull-Down Resistor:
Connect a 10 kOhm resistor between pin `RC6` and `GND`.
If a wire breaks or disconnects during vehicle operation, the resistor pulls the pin voltage to 0 V.
This immediately stops the motor.

---

## 5. Real-Time Serial Telemetry

The firmware outputs real-time operational data through `UART1` (pin RC8 TX) at 115200 baud.
The telemetry message transmits at a rate of 10 Hz (every 100 ms).

### Data Format:
```
POT:12400 POT_mV:1248 THR%:36 RUN:1 STATE:3 RPM:0 DIR:1 FLT:0 VBUS_mV:54008 IQ_mA:0 ID_mA:0 IQR_mA:0 VQ_mV:0 VMAX_mV:29606 SAT:0
```

### Field Definitions:
- **POT**: Raw 15-bit throttle ADC reading (range: `0` to `32767`).
- **POT_mV**: Throttle input voltage in millivolts (range: `0` to `3300` mV).
- **THR%**: Commanded throttle percentage (range: `0` to `100`%).
- **RUN**: Motor activation status (`0` = stopped / freewheel, `1` = running).
- **STATE**: Internal state (`0` = Restart, `1` = Stopped, `2` = Starting, `3` = Running, `4` = Stopping, `5` = Fault).
- **RPM**: Estimated mechanical speed (calibrate against a tachometer).
- **DIR**: Fixed commanded rotation direction (`1` = forward). The firmware does not auto-correct phase/Hall wiring.
- **FLT**: Fault bitmask (`0` = none, bit 0 = undervoltage, bit 1 = overvoltage, bit 2 = overtemperature, bit 3/4/5 = overcurrent A/B/C).
- **VBUS_mV**: Measured DC bus voltage in millivolts (nominal 54 000 mV).
- **IQ_mA**: Measured q-axis (torque-producing) current in milliamps.
- **ID_mA**: Measured d-axis current in milliamps (normally near zero).
- **IQR_mA**: q-axis current reference in milliamps. At full throttle this should be about 5478 mA (the 5.48 A peak cap).
- **VQ_mV**: q-axis voltage command in millivolts (line-neutral peak).
- **VMAX_mV**: q-axis voltage limit in millivolts. If `VQ_mV` reaches `VMAX_mV`, the drive is voltage-saturated, not current-limited.
- **SAT**: Saturation state (`0` = none, `1` = voltage, `2` = current, `3` = both). The current thresholds are inherited and only indicate very large currents, so use `IQ_mA` vs `IQR_mA` for the real current-limit check.

### Current Units (important when using a clamp meter):
The `5.48 A` limit in the firmware is a **peak phase-current** limit, because the FOC Park transform produces the peak of the sinusoidal phase current. A clamp meter on one motor phase reads **RMS**, so a controller sitting exactly at the 5.48 A peak limit shows about **3.9 A RMS** (plus a little ripple/harmonic content, so 4.0–4.2 A is normal).

Maxon's 5.48 A nominal current is a **DC/terminal** current, not an RMS phase current. In sinusoidal FOC, the peak phase current that produces the same nominal torque (627 mNm) is about **6.0 A peak** (≈4.24 A RMS). The current firmware cap is therefore about 9 % below the motor's nominal torque. Raising it to 6.0 A peak would recover the full nominal torque; the clamp would then read about 4.2–4.4 A RMS, still not 5.48 A.

### Single Tune with Automatic Soft Start:
There is one throttle map and one current limit. Nothing is rider-selectable.

| Item | Value |
|---|---|
| Minimum current | 0 A (no low-end step) |
| Maximum current | 5.48 A (hard cap) |
| Throttle curve | Quadratic |
| Torque rise/fall ramp | 15 A/s each |
| Throttle filter | 20 ms low-pass |
| Soft-phase gains | 50 % Kp / 50 % Ki |
| Full gains | 100 % Kp / 100 % Ki |

The soft gains are active for **500 ms** after the throttle is applied, then fade to the full gains over **300 ms**.
This repeats on every start; a throttle release, a stop or a fault re-arms it.
The soft ramp and gains remove the launch shock without reducing the 5.48 A ceiling.

### Interface Selection (ESP32 UART Telemetry):
The firmware is fixed to `TELEMETRY_UART`. UART1 carries only the human-readable text telemetry for the ESP32; X2CScope initialization and communication are disabled.

```c
#define TELEMETRY_MODE  TELEMETRY_UART
```

The text stream is non-blocking and is sent every `TELEMETRY_PERIOD_MS` (default 100 ms) at 115200 baud, 8-N-1.

---

## 6. How to View the Potentiometer Value

Follow these steps to monitor the potentiometer in real time:

1. Connect a USB-to-UART adapter to the controller:
   - Connect adapter `RX` to controller `RC8` (TX).
   - Connect adapter `GND` to controller `GND`.
2. Insert the USB adapter into your computer.
3. Open a serial terminal program (for example: VS Code Serial Monitor, PuTTY, or Tera Term).
4. Configure the serial port settings:
   - **Baud Rate**: `115200`
   - **Data Bits**: `8`
   - **Parity**: `None`
   - **Stop Bits**: `1`
5. Turn the potentiometer knob.
   Observe the `POT` value and `POT_mV` value change on the screen in real time.

### Automated UART Capture

A complete field dictionary for the ESP32 stream is available in `docs/TELEMETRY_DATA_DICTIONARY.md`.

A PowerShell logger is included at `tools/capture_telemetry.ps1`. It records the complete telemetry stream to CSV and prints each line to the console.

Example:

```powershell
.\tools\capture_telemetry.ps1 -PortName COM5 -DurationSeconds 30 -CsvPath .\test_run.csv
```

Replace `COM5` with the USB-to-UART adapter's actual COM port. The CSV can be opened in Excel or any plotting tool for current, voltage, speed, and fault analysis.
