# ESP32 UART Telemetry Data Dictionary

## 1. Transport

| Item | Value |
|---|---|
| Interface | dsPIC33CK UART1 TX |
| dsPIC pin | `RC8` |
| ESP32 connection | ESP32 RX |
| Ground | Common `GND` |
| Baud rate | `115200` |
| Format | `8-N-1` |
| Encoding | ASCII |
| Line ending | CRLF (`\r\n`) |
| Update rate | 10 Hz, one line every 100 ms |
| Owner | UART text telemetry only; X2CScope is disabled |

Each line is a space-separated list of `KEY:VALUE` fields. A host logger may add its own timestamp; the controller does not send one.

## 2. Example Line

```text
POT:15230 POT_mV:1533 THR%:100 RUN:1 STATE:3 RPM:1500 DIR:1 FLT:0 VBUS_mV:54008 IQ_mA:5478 ID_mA:12 IQR_mA:5478 VQ_mV:3270 VMAX_mV:29606 SAT:0
```

## 3. Field Dictionary

| # | Key | Type | Unit / Range | Meaning |
|---:|---|---:|---|---|
| 1 | `POT` | integer | `0`–`32767` counts | Raw throttle ADC reading. The low-pass filter is applied later, before the throttle percentage is calculated. |
| 2 | `POT_mV` | integer | `0`–`3300` mV | Throttle voltage calculated from `POT`. |
| 3 | `THR%` | integer | `0`–`100` percent | Throttle position percentage. Because the throttle-to-current map is quadratic, this is **not** the percentage of motor current. |
| 4 | `RUN` | integer | `0` or `1` | Inverter enable request. `1` means the throttle is requesting motor operation. |
| 5 | `STATE` | integer | `0`–`5` | Firmware state machine state. |
| 6 | `RPM` | integer | `0`–about `4861` rpm | Absolute estimated mechanical speed from the Hall tracking loop. The telemetry scale saturates near 4861 rpm. |
| 7 | `DIR` | integer | `1` | Fixed commanded forward direction. The firmware does not auto-correct phase or Hall wiring. |
| 8 | `FLT` | integer bitmask | `0`–`63` | Latched fault flags. |
| 9 | `VBUS_mV` | integer | about `43200`–`63500` mV | Measured DC bus voltage. Nominal pack is about `54000` mV. |
| 10 | `IQ_mA` | signed integer | mA | Measured q-axis phase current. This is the torque-producing component of the FOC current vector. |
| 11 | `ID_mA` | signed integer | mA | Measured d-axis phase current. Normally near zero; it may become negative during high-speed field weakening. |
| 12 | `IQR_mA` | signed integer | mA | Commanded q-axis current reference. Full throttle is approximately `5478` mA. |
| 13 | `VQ_mV` | signed integer | mV | Commanded q-axis voltage, line-neutral peak. |
| 14 | `VMAX_mV` | signed integer | mV | Current q-axis voltage headroom/limit. If `VQ_mV` reaches `VMAX_mV`, the current loop is voltage-saturated. |
| 15 | `SAT` | integer | `0`–`3` | Latched saturation-state indicator. |

## 4. Enumerations

### `STATE`

| Value | Name | Meaning |
|---:|---|---|
| `0` | `MCAPP_RESTART` | Current-offset calibration/bootstrap restart. |
| `1` | `MCAPP_STOPPED` | PWM inhibited; motor stopped. |
| `2` | `MCAPP_STARTING` | Startup/transition active. |
| `3` | `MCAPP_RUNNING` | Normal closed-loop FOC operation. |
| `4` | `MCAPP_STOPPING` | Throttle released; coasting/stop transition. |
| `5` | `MCAPP_FAULT` | Protective shutdown. |

### `FLT` Bitmask

| Bit | Value | Fault |
|---:|---:|---|
| 0 | `1` | Undervoltage |
| 1 | `2` | Overvoltage |
| 2 | `4` | MOSFET overtemperature |
| 3 | `8` | Phase A overcurrent |
| 4 | `16` | Phase B overcurrent |
| 5 | `32` | Phase C overcurrent |

Examples:

- `FLT:0` — no active fault.
- `FLT:1` — undervoltage.
- `FLT:4` — overtemperature.
- `FLT:9` — undervoltage plus phase A overcurrent.

### `SAT`

| Value | Meaning |
|---:|---|
| `0` | No saturation indicator |
| `1` | Voltage saturation |
| `2` | Current saturation |
| `3` | Voltage and current saturation |

The current-saturation thresholds were inherited from a much larger motor and are not useful for detecting the 5.48 A firmware cap. Use `IQ_mA` versus `IQR_mA` for the real current-limit check.

## 5. Interpretation Guide

- **Current limit:** `IQR_mA` near `5478` means full commanded current. If `IQ_mA` follows `IQR_mA`, the current loop is delivering the request.
- **Voltage saturation:** `VQ_mV` close to `VMAX_mV` means the drive is out of voltage headroom.
- **Field weakening:** Above about 4000 rpm, `ID_mA` may become negative, up to about `-1000 mA`, while `IQ_mA` remains limited so the total current vector stays within the firmware cap.
- **Healthy running line:** Usually `RUN:1`, `STATE:3`, `DIR:1`, `FLT:0`.
- **Fast events:** The 10 Hz telemetry is diagnostic, not a high-speed oscilloscope. Events shorter than 100 ms may be missed.

## 6. Throttle Calibration Reference

| Threshold | Counts | Approximate voltage | Meaning |
|---|---:|---:|---|
| Rest / deadband | `7547` | 760 mV | `THR%:0`, motor request off |
| Re-engage point | `7747` | 780 mV | `THROTTLE_LOW + THROTTLE_HYSTERESIS` |
| Full throttle | `23500` | 2370 mV | `THR%:100`, maximum torque command |

The installed throttle calibration is defined in `src/userparms.h`.

## 7. ESP32 Parsing Notes

- Parse all values as signed integers; the firmware sends ASCII decimal text.
- Split the line on spaces.
- For each token, split at the first `:` only.
- Ignore unknown keys so future firmware revisions can add fields.
- Do not assume field order when parsing by key.
- CRLF may arrive as either `\r\n` or a split packet; buffer until `\n`.
