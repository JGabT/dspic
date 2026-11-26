# System Description

## 1. Overview

This document describes the motor control firmware for the dsPIC33CK256MP505 microcontroller.
The firmware controls a Maxon EC 48 V (part 634043) permanent magnet synchronous motor in a Shell Eco-marathon Battery Electric vehicle.
The motor is the 48 V variant: 8 pole pairs, 3610 rpm nominal, 3970 rpm no-load, 627 mNm nominal torque, and 5.48 A nominal (maximum continuous) current.
The vehicle battery pack operates at 54 V nominal DC voltage.

The firmware uses Field-Oriented Control (FOC).
FOC gives high electrical efficiency, low acoustic noise, and smooth torque response.
The d/q voltage limit now uses the full linear SVPWM range, and a 1 A d-axis current is applied gradually above 4000 rpm to extend the speed range without exceeding the motor's 5000 rpm mechanical maximum.

---

## 2. Hardware Specifications

| Item | Specification |
| :--- | :--- |
| **Microcontroller** | Microchip dsPIC33CK256MP505 |
| **Motor** | Maxon EC 48 V, part 634043 |
| **Pole Pairs** | 8 |
| **Nominal Motor Voltage** | 48 V DC |
| **Nominal Speed** | 3610 rpm (3970 rpm no-load) |
| **Nominal Torque** | 627 mNm |
| **Nominal Current** | 5.48 A (maximum continuous) |
| **Phase Resistance** | 0.236 ohm (phase-to-phase) |
| **Phase Inductance** | 0.424 mH (phase-to-phase) |
| **Max Permissible Speed** | 5000 rpm (no software limit) |
| **Nominal Motor Power** | ~237 W mechanical / ~263 W electrical |
| **Nominal Bus Voltage** | 54 V DC pack (63 V maximum) |
| **Overvoltage Threshold** | 63.5 V DC |
| **Undervoltage Threshold** | 43.2 V DC |
| **PWM Switching Frequency** | 20 kHz (50 microseconds cycle time) |
| **Throttle Input** | Analog potentiometer (0 V to 3.3 V DC) |
| **Position Sensors** | 3 Digital Hall Sensors |

### Motor and Hall Wiring (Maxon EC 48 V):
- Motor cable: red = winding 1, black = winding 2, white = winding 3.
- Hall cable: yellow = Hall 1, brown = Hall 2, grey = Hall 3, blue = GND, green = +4.5 to +24 V.
- Verify the board's phase order and `IO_HALL1/2/3` mapping against the motor. If the motor runs rough or backwards, swap two motor phases and/or correct the Hall mapping.
- The Maxon Hall sensors are not 3.3 V devices. Use the board's Hall supply/level shifting and verify the dsPIC input levels before connecting them.

### Fixed Hall/Phase Direction:
- The firmware uses a fixed forward direction and the configured Hall table (`1 -> 3 -> 2 -> 6 -> 4 -> 5 -> 1`).
- It does not learn or automatically correct phase/Hall wiring.
- `DIR:1` in telemetry is the fixed commanded direction. If the motor runs backward or produces the wrong torque, correct the Hall mapping or swap two motor phases.
- `RPM` is estimated from the Hall tracking loop. The firmware does not impose a speed limit; the motor maximum is 5,000 rpm.

---

## 3. Microcontroller Pin Assignments

| Pin Name | Function | Direction | Description |
| :--- | :--- | :--- | :--- |
| **RC6 (AN17)** | Throttle Input | Input (Analog) | Potentiometer wiper signal |
| **RC8 (RP56)** | UART1 TX | Output (Digital) | Telemetry transmit to serial monitor |
| **RC9 (RP57)** | UART1 RX | Input (Digital) | Serial receive line |
| **RA0 (AN0)** | Phase A Current | Input (Analog) | Inverter current shunt feedback |
| **RA4 (AN4)** | Phase B Current | Input (Analog) | Inverter current shunt feedback |
| **RB15 (AN15)**| DC Bus Voltage | Input (Analog) | Scaled DC battery voltage |
| **RB12 (AN12)**| Heatsink Temp | Input (Analog) | Inverter temperature sensor |
| **RC3 (PWM1H)**| Inverter High A | Output (PWM) | High-side gate drive Phase A |
| **RC4 (PWM1L)**| Inverter Low A | Output (PWM) | Low-side gate drive Phase A |
| **RD1 (PWM2H)**| Inverter High B | Output (PWM) | High-side gate drive Phase B |
| **RD2 (PWM2L)**| Inverter Low B | Output (PWM) | Low-side gate drive Phase B |
| **RD3 (PWM3H)**| Inverter High C | Output (PWM) | High-side gate drive Phase C |
| **RD4 (PWM3L)**| Inverter Low C | Output (PWM) | Low-side gate drive Phase C |

---

## 4. Software State Machine

The firmware uses a state machine to control motor operations safely:

```
[ MCAPP_RESTART ]
       |
       v  (Current offset calibration complete)
[ MCAPP_STOPPED ] <-----------------------+
       |                                  |
       v  (Throttle > Deadband AND Armed) | (Throttle released)
[ MCAPP_STARTING ]                        |
       |                                  |
       v                                  |
[ MCAPP_RUNNING ] ------------------------+
```

### State Descriptions:
1. **MCAPP_RESTART**: The system calibrates ADC current offsets. The inverter gate outputs stay disabled.
2. **MCAPP_STOPPED**: The motor does not turn. Inverter outputs are overridden to low (high impedance).
3. **MCAPP_STARTING**: The system initializes closed-loop FOC and aligns rotor position.
4. **MCAPP_RUNNING**: The system applies sinusoidal phase currents to produce commanded torque.
5. **MCAPP_FAULT**: The system detects an overcurrent, overvoltage, or undervoltage condition. The system immediately shuts off all gate signals.
