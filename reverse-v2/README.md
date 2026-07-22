# reverse-v2 — clean re-disassembly of the MI 302 mower firmware (2026-07-02)

A full **repeat** decompilation from scratch, without trusting the first iteration (`docs/fw/`, `docs/fw-chip2/`, `docs/live-findings.md`).
Tool: **Ghidra 12.1.2 native decompiler** (not r2 pdg) in Docker + a peripheral map from the GD32F30x_CL SVD.

## What is here
- `chip1/` — GD32F305 (main board, 1 MB). **1931 functions, 100% decompiled.**
- `chip2/` — GD32F30x (coil board, 256 KB). **435 functions, 100% decompiled.**
- Each folder: `decompiled_all.c` (all pseudo-C), `functions_index.csv`, `symbols.txt`, `strings.txt`, `memory_map.txt`.
- `ghidra-scripts/` — headless scripts (Prep.java: SVD map+SRAM+vector seeding; ExportDecomp.java: export).
- `GD32F30x_CL.svd` — the peripheral map (62 blocks, 1026 named registers applied).

## How to reproduce (Docker, no runtimes on the host)
```
docker run --rm -v <ws>:/work -e MAXMEM=6G \
  --entrypoint /ghidra/support/analyzeHeadless blacktop/ghidra:latest \
  /work/proj chip1 -import /work/in/chip1.bin \
  -processor "ARM:LE:32:Cortex" -loader BinaryLoader -loader-baseAddr 0x08000000 \
  -scriptPath /work/scripts \
  -preScript Prep.java /work/svd/GD32F30x_CL.svd \
  -postScript ExportDecomp.java /work/out-chip1 -deleteProject
```

## Confirmed by this iteration (✔ = from fresh code/strings)
- ✔ Both dumps are valid **unencrypted** Cortex-M4 firmware, vector table @0x08000000.
  chip1: SP=0x20016ed8, Reset=0x08011a3c. chip2: SP=0x20000988, Reset=0x080001b4.
- ✔ chip1 — a multi-faceted system: **bootloader + IAP/OTA** of several images (ENV tags `BB_*`, `DB_*`, `LB_*` = separate MCU images: mainboard / display / led), EasyFlash ENV, FreeRTOS, cJSON.
- ✔ Peripherals per the SVD: ADC0 @0x40012400, TIMER0 @0x40012c00, TIMER7 @0x40013400, USART0 @0x40013800, CRC @0x40023000 — all as volatile MMIO.

## ⚠️ Discrepancies with the first iteration (being re-checked, Phase 3/4)
- ⚠️ **The application CRC is NOT hardware.** The hardware CRC block (0x40023000) is **never used** in the code. The first iteration claimed "HW CRC-32 @0x40023000 poly 0x04C11DB7". → the CRC is software/table-based, so the recomputation on a patch will be different.
- ⚠️ **The CRC is verified in the OTA-update path**, and not (not yet proven) on every boot. Three gate functions `FUN_08006658/08006ac0/…` compare the computed CRC of a freshly flashed block with the expected one from ENV (`BB_CRC`/`DB_CRC`/`LB_CRC`) and print `CRC check error: app CRC / get CRC`. → we need to prove whether there is a boot-time check before jumping into the app.
- ⚠️ **The application base ≠ confirmed as 0x08018000** — there are no direct references. The bootloader→app jump mechanism has not yet been localized.
- The former "CCR TIMER2 = wheel steering", the motor/odometry/state addresses — **not re-confirmed**, awaiting Phase 3 (check against a live chip).

## Next
Phase 3: check every old claim against this code (✔/ⓗ/❌).
Phase 4: the exact CRC/boot mechanism → the patch→recompute→flash procedure + rollback.
Phase 5: manual/standalone control + firmware tools and instructions.
