# System Description

## 1. Overview

This document describes the motor control firmware for the dsPIC33CK256MP505 microcontroller.
The firmware controls a 250 W permanent magnet motor in a Shell Eco-marathon Battery Electric vehicle.
The vehicle electrical system operates at 54 V nominal DC voltage.

The firmware uses Field-Oriented Control (FOC).
FOC gives high electrical efficiency, low acoustic noise, and smooth torque response.

---

## 2. Hardware Specifications

| Item | Specification |
| :--- | :--- |
| **Microcontroller** | Microchip dsPIC33CK256MP505 |
| **Nominal Bus Voltage** | 54 V DC |
| **Overvoltage Threshold** | 62.6 V DC |
| **Undervoltage Threshold** | 43.2 V DC |
| **Continuous Motor Power** | 250 W |
| **PWM Switching Frequency** | 20 kHz (50 microseconds cycle time) |
| **Throttle Input** | Analog potentiometer (0 V to 3.3 V DC) |
| **Position Sensors** | 3 Digital Hall Sensors |

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
