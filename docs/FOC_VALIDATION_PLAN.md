# Maxon EC-i 52 FOC Validation Plan

## 1. Safety Setup

- Use a current-limited 54 V bench supply when possible.
- Keep the 5.48 A firmware phase-current cap enabled.
- Secure the motor and sprocket. Remove loose tools and clothing.
- Start every test at low throttle with no load.
- Stop immediately if the controller faults repeatedly, the motor stalls at high current, or the bus voltage rises unexpectedly.
- Do not exceed the motor's 5000 rpm mechanical maximum.

## 2. Required Measurements

- Controller telemetry at 115200 baud, 8-N-1.
- External tachometer or optical speed measurement.
- DC bus voltage.
- Phase-current waveform or clamp meter for RMS checks.
- Oscilloscope access to Hall 1/2/3 and at least one gate-drive output when available.

Capture telemetry with:

```powershell
.\tools\capture_telemetry.ps1 -PortName COM5 -DurationSeconds 30 -CsvPath .\validation.csv
```

Replace `COM5` with the adapter's actual port.

## 3. Baseline Test

Flash the known-good backup HEX and record the following before making further changes.

| Throttle | Load | Record |
|---:|---|---|
| 5% | None | RUN, STATE, RPM, IQ, IQR, VQ, VMAX, FLT |
| 10% | None | Same |
| 25% | None | Same |
| 50% | Sprocket/load | Same |
| 75% | Sprocket/load | Same |
| 100% | None, then load | Same plus VBUS and external tachometer |

Check:

- `STATE` remains stable while the throttle is held.
- `FLT` remains zero.
- `IQ_mA` follows `IQR_mA` when voltage headroom is available.
- `VQ_mV` approaching `VMAX_mV` identifies voltage saturation.
- Telemetry RPM agrees with the external tachometer.

## 4. Hall Validation

1. Record the raw Hall state sequence while the rotor is turned slowly by hand in the forward direction.
2. Expected sequence: `1 -> 3 -> 2 -> 6 -> 4 -> 5 -> 1`.
3. Scope the three Hall signals at the dsPIC pins.
4. Reject any test where states `0` or `7` persist, edges are missing, or signal levels do not reach valid logic thresholds.
5. Verify the electrical-angle offset at low speed before trusting high-speed current-loop measurements.

## 5. Current-Loop Validation

- Apply a small, stable torque command at low speed.
- Compare measured `IQ_mA` with `IQR_mA`.
- Look for periodic current oscillation at the electrical fundamental, sixth harmonic, PWM frequency, or a subharmonic.
- Verify that current-sense offsets are calibrated with PWM disabled.
- Confirm ADC sampling occurs while both sensed low-side devices are on.
- Retune Kp/Ki only after Hall angle and ADC timing are verified.

## 6. High-Speed Voltage Validation

- Increase throttle gradually on a secured motor.
- Record RPM, VBUS, IQ, IQR, VQ, and VMAX together.
- Identify the speed where VQ reaches VMAX.
- Verify that the 1 A field-weakening ramp above 4000 rpm remains smooth and does not exceed 5000 rpm.
- Check that phase current remains below the 5.48 A peak firmware cap.

## 7. Restart and Throttle Validation

Test all of the following:

- Power-on with throttle above zero: motor must remain inhibited.
- Normal launch from zero throttle.
- Release and reapply while the rotor is coasting.
- Fixed throttle at 10%, 25%, 50%, 75%, and 100%.
- Slow roll-off from full throttle.
- Fault recovery with the actual fault cause removed.

Record any jerk, stutter, or on/off sensation together with the telemetry timestamp.

## 8. Acceptance Criteria

The controller is considered validated when:

- No unintended faults occur during the test matrix.
- Telemetry RPM matches an external tachometer.
- Hall states are valid and the angle offset is stable.
- IQ tracks IQR except when intentional voltage/current limiting is identified.
- Throttle response is progressive and repeatable.
- No repeated fault/restart cycling occurs.
- Maximum measured speed remains below 5000 rpm.
- Bus voltage remains below 63.5 V.
