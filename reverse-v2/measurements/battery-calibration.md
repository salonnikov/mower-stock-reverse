# Battery calibration — live points from the factory firmware

Captured over SWD (non-halt) from the running factory firmware of chip1. The ADC0 DMA-scan buffer
@`0x2000bb34` (address from DMA0 CH0 MADDR 0x40020014), RL=5, 16-bit per channel.
Rank order (probable, ⚠️ rank→channel not exactly verified): 0=ch15/PC5 pack, 1=ch14/PC4 current,
2=ch16 temp, 3=ch9/PB1 OCV, 4=ch17 Vref.

| Date | Display | rank0 (PC5 pack, raw) | pack_mV (=raw×5.4277) | rank1 current | rank3 OCV | Note |
|---|---|---|---|---|---|---|
| 2026-07-06 | **100%** | 0x0E82 = **3714** | ~20 160 mV (~20.16 V) | 0x0004≈0 | 0x0DA0=3488 | not charging (current≈0), state=3 |

## Conclusion for our firmware (firmware/mower-own/src/battery.c)
- `BATT_MV_FULL = 21000` is too high: at a real 100% the pack = ~20 160 mV → our formula would give ~86%.
- Recalibrate: 100% ≈ 20 200 mV (from this point). One more point at a low charge is needed for the slope.
- ⚠️ the rank→channel order is not 100% confirmed — verify on the next session (e.g. by the change of rank1 while charging = current rises).
