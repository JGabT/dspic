# Shell Eco-Marathon 250W Optimization Guide

## 1. Overview

This document describes methods to maximize vehicle energy efficiency in the Shell Eco-marathon Battery Electric category.
The objective is to travel the competition course with the lowest electrical energy consumption (Watt-hours).

---

## 2. Electrical Power and Current Limits

The prototype vehicle uses a nominal 54 V DC battery and a 250 W electric motor.

### Current Limit Calculations:
- **Nominal Continuous Power**: 250 W
- **Nominal DC Voltage**: 54 V
- **Continuous Current**:
  $$\text{Current} = \frac{250\text{ W}}{54\text{ V}} \approx 4.63\text{ A}$$
- **Peak Acceleration Current (Burn Phase)**:
  $$\text{Peak Current} = \frac{375\text{ W}}{54\text{ V}} \approx 7.0\text{ A}$$

The firmware configures these limits in `src/userparms.h`:
```c
#define MAX_TORQUE_CURRENT    NORM_CURRENT (7.0) // 7.0 A Peak Limit
#define MIN_TORQUE_CURRENT    NORM_CURRENT (0.5) // 0.5 A Minimum Torque
```

### Why These Limits Increase Efficiency:
- Standard e-bike firmware uses limits between 15 A and 30 A.
- High current causes large resistive heating losses ($I^2 \times R$) in the motor windings and battery pack.
- Limiting current to 7.0 A prevents winding overheating and saves battery energy.

---

## 3. "Burn and Coast" Driving Strategy

The "Burn and Coast" technique gives the highest vehicle fuel economy:

```
Speed
  ^
  |        Burn (7.0A)              Burn (7.0A)
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
   - The firmware detects `potValue < THROTTLE_LOW` and sets `runMotor = 0`.
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
