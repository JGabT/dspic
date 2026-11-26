# Shell Eco-Marathon Maxon EC 48 V Optimization Guide

## 1. Overview

This document describes methods to maximize vehicle energy efficiency in the Shell Eco-marathon Battery Electric category.
The objective is to travel the competition course with the lowest electrical energy consumption (Watt-hours).

---

## 2. Electrical Power and Current Limits

The prototype vehicle uses a nominal 54 V DC battery and a Maxon EC 48 V (part 634043) motor.

### Current Limit Calculations:
- **Nominal continuous current (motor datasheet)**: 5.48 A
- **Nominal motor voltage**: 48 V DC
- **Nominal electrical power**: 5.48 A × 48 V ≈ 263 W (≈237 W mechanical)
- **Maximum current**: 5.48 A (hard cap, single tune)

The firmware configures this limit in `src/userparms.h` and `src/motor_drive.c`:
```c
#define MAX_TORQUE_CURRENT     NORM_CURRENT(5.48) // hard cap
/* Ceiling in motor_drive.c: */
#define TUNE_MAX_CURRENT       (MAX_TORQUE_CURRENT)
```

**Unit note:** `NORM_CURRENT(5.48)` is a **peak phase-current** limit in the FOC code.
A clamp meter on one motor phase reads RMS, so it shows about **3.9 A** when the controller is at this limit.
Maxon's 5.48 A datasheet value is a **DC/terminal** current; the FOC peak current that produces the same 627 mNm nominal torque is about **6.0 A**.
See [Throttle Calibration & Telemetry](THROTTLE_CALIBRATION_AND_TELEMETRY.md) for the full explanation.

### Why These Limits Increase Efficiency:
- Standard e-bike firmware uses limits up to 30 A.
- High current causes large resistive heating losses ($I^2 \times R$) in the motor windings and battery pack.
- A 5.48 A hard cap keeps the motor inside its thermal envelope. A single automatic soft-start ramp removes launch shock without raising the current.

---

## 3. "Burn and Coast" Driving Strategy

The "Burn and Coast" technique gives the highest vehicle fuel economy:

```
Speed
  ^
  |        Burn (5.48 A)              Burn (5.48 A)
V_max |       /\                       /\
      |      /  \                     /  \
      |     /    \                   /    \
V_min |    /      \                 /      \
      |   /        \ Coast (0.0A)  /        \ Coast (0.0A)
  0   +--+----------+-------------+----------+------------> Time
```

### Phase Instructions:
1. **Burn Phase (Acceleration)**:
   - Turn the throttle to accelerate the vehicle from low speed ($V_{\text{min}}$, e.g. 20 km/h) to top speed ($V_{\text{max}}$, e.g. 32 km/h).
   - The motor operates near its peak electrical efficiency band (typically 80% to 88% efficiency).
2. **Coast Phase (Freewheeling)**:
   - Release the throttle completely.
   - The firmware detects `potValue <= THROTTLE_LOW` and sets `runMotor = 0`.
   - The controller executes `DisablePWMOutputsInverterA()`.
   - All inverter MOSFET switches turn off.
   - The motor freewheels with zero electromagnetic braking.
   - The vehicle coasts until speed drops to $V_{\text{min}}$.
3. **Repeat**:
   - Turn the throttle again to repeat the cycle.

---

## 4. Technical Scrutineering Checklist

Make sure your vehicle satisfies these Shell Eco-marathon rules:

- [ ] **Zero-Throttle Start**: The motor does not rotate when the battery main switch is turned on with the throttle depressed.
- [ ] **Spring Return**: The throttle pedal or hand grip returns to zero automatically when released.
- [ ] **Emergency Shutdown**: The manual emergency kill-switch disconnects battery power cleanly.
- [ ] **Electrical Insulation**: All 54 V power cables have appropriate insulation, strain relief, and fuse protection.
- [ ] **Fail-Safe Potentiometer**: A 10 kOhm pull-down resistor pulls pin RC6 to ground if the wiper wire disconnects.
