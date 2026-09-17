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
3. If the throttle voltage is higher than 0.25 V (2482 counts), the motor does not turn.
4. The driver must release the throttle to the resting position (below 0.25 V).
5. When the firmware reads a voltage below 0.25 V, the safety interlock arms the motor.
6. The driver can then turn the throttle to accelerate the vehicle.

---

## 3. Throttle Calibration Thresholds

The file `src/userparms.h` defines the throttle calibration thresholds:

```c
#define THROTTLE_LOW        (int)((0.25/3.3)*32767) // 0.25V (~2482 counts) - Lower Deadband
#define THROTTLE_HIGH       (int)((3.00/3.3)*32767) // 3.00V (~29788 counts) - Full Throttle
#define THROTTLE_HYSTERESIS (int)((0.08/3.3)*32767) // 0.08V (~794 counts) - Anti-chatter Window
```

### Threshold Definitions:
- **THROTTLE_LOW (0.25 V / 2482 counts)**:
  Signals below this voltage are in the resting deadband.
  The controller commands zero torque.
- **THROTTLE_HIGH (3.00 V / 29788 counts)**:
  Signals at or above this voltage command 100% of maximum configured torque (7.0 A).
- **THROTTLE_HYSTERESIS (0.08 V / 794 counts)**:
  Prevents motor chatter when the throttle rests near the deadband edge.
  The motor activates at `0.33 V` (2482 + 794 counts).
  The motor deactivates at `0.25 V` (2482 counts).

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
POT:12400 V_mV:1248 THR%:36 RUN:1 STATE:2
```

### Field Definitions:
- **POT**: Raw 15-bit ADC reading (range: `0` to `32767`).
- **V_mV**: Input throttle voltage in millivolts (range: `0` to `3300` mV).
- **THR%**: Commanded throttle percentage (range: `0` to `100`%).
- **RUN**: Motor activation status (`0` = stopped / freewheel, `1` = running).
- **STATE**: Internal state (`0` = Restart, `1` = Stopped, `2` = Starting/Running).

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
   Observe the `POT` value and `V_mV` value change on the screen in real time.
