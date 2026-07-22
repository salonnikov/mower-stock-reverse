# Audit of the "wheels do not spin" root cause — 2026-07-10

## Conclusion

The cause is still **not proven**. The claim "the wheel rail 20U is off" cannot be treated as an established fact: there is no direct measurement of VBB, and `diag=0xFFFF` by itself does not distinguish an absence of power, an incorrect SPI configuration, a MISO problem, or an inactive A4963.

On a re-check, missed differences between the factory and standalone firmware were found. These must be checked and closed first, before transferring the cause to a hardware high-side gate.

## 1. The fast/slow A4963 configuration selection is not checked

The factory firmware reads `FUN_08050578()[0x54]` and selects a configuration set:

| Mode | CONFIG0..5 |
|---|---|
| fast | `03E8 22DF 4753 6721 8735 A736` |
| slow | `01A8 2ADF 47D3 6721 8745 A736` |

Our firmware always uses the fast set: `firmware/mower-own/src/motor.c`, `s_a4963_cfg`.

The factory `platform[0x54]` value for this specific board is not recorded. If the factory runs in the slow variant, we flash the wrong CONFIG0/1/2/4 into the wheel A4963. The blade working does not disprove this: the blade has a different config and a separate power path.

## 2. The standalone firmware SPI mode is not confirmed byte-for-byte

Our firmware hard-sets `SPI_CTL0 = 0x0B2F` in `firmware/mower-own/src/hal.c`.

In the factory `FUN_0801f2f4` the SPI is configured through a parameter struct and the HAL: `FUN_0807dde2` → `FUN_0807dd88`. The exact final CTL0/CTL1 state is not reduced to a proven comparison against our firmware in the active materials.

Therefore "the SPI transaction completed" is not equal to "the A4963 accepted the configuration". We need to obtain and compare the factory CTL0, CTL1, STAT, the CS/NSS order, and the 16-bit frame on a clean init.

## 3. A full TIMER2 match is not proven

The correct channels and duty values are confirmed:

- CH1 — blade;
- CH2 — right wheel;
- CH3 — left wheel.

But for the standalone firmware and the factory firmware, a full match of `TIMER2_CTL0`, `CHCTL0`, `CHCTL1`, `CHCTL2`, `PSC`, `CAR`, preload/output-enable and the AFIO remap at the moment of rotation is not recorded. A match of `CH2CV/CH3CV` alone is insufficient: the CCR can change while the output channel is inactive or misconfigured.

## 4. What can already be considered closed

- PWM channels: TIMER2 CH2/CH3 for the wheels, CH1 for the blade.
- Wheel CS binding: PD5/PD4; blade: PD3.
- The A4963 frame format and the general path `FUN_0808408c` are studied.
- PD7/PD4 as run/coast were already checked in the LOW state and did not explain the problem.
- TIMER0 as the motor timer is ruled out.
- BMS keep-alive was checked earlier; do not treat it as a current open direction.
- The top-level ESP32/ESPHome stubs are irrelevant to this problem.

## Correct order of the next check

1. On the factory firmware, read the actual `platform[0x54]` and the real CONFIG0..7 of the left and right A4963 after init.
2. Compare these words with what `mower-own` sends.
3. Capture the factory SPI1 CTL0/CTL1/STAT and the full TIMER2 state at the moment the wheels physically rotate.
4. Compare with the same registers of the standalone firmware at a non-zero duty.
5. Only if these checks match and the wheels still do not work, return to the hardware VBB/20U circuit and measure it directly.

## Sources

- `reverse-v2/reference/A4963-DRIVER-bringup-ram.md`
- `reverse-v2/factory-map/04-motor-drivers.md`
- `reverse-v2/reference/PERIPHERALS-chip1.md`
- `reverse-v2/reports/drive-chain.md`
- `reverse-v2/measurements/force-capture-2026-07-06.txt`
- `firmware/mower-own/src/motor.c`
- `firmware/mower-own/src/hal.c`
