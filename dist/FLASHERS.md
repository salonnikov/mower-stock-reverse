# chip1 flashers and images — which are correct, which are not

Reorganization 2026-07-06. Result of a multi-agent review (which cfg is working/broken).

## Canonical images (raw material, git-tracked — DO NOT lose)

| File | What | sha256 | Note |
|---|---|---|---|
| `gd32-mainboard-dump-v1.bin` | **Raw chip1 dump** (1 MB, base 0x08000000) — the revert reference | `82b8bb5a…c4c5c` | == the current factory flash at the time of the dump |
| `gd32-mainboard-chip2-dump-v1.bin` | **Raw chip2 dump** (256 KB) | `71a9801c…f3ef` | coil board |
| `factory-full.asm` | Full chip1 disasm (17 MB) | — | for byte comparison |
| `own_fw.bin` | **Our firmware** (image in place of app 0x08018000) | `78b03536…e704c` | NOT flashed |

Decompilation of both chips (canonical): `reverse-v2/chip1/` and `reverse-v2/chip2/`.

## Flashers — STATUS

| File | Purpose | Status |
|---|---|---|
| `dist/flash_own.cfg` | Flash our firmware (own_fw) | ✅ **WORKING** (main path, verify+abort-without-reset) |
| `dist/flash_revert_own.cfg` | Revert: ours → factory | ✅ **WORKING revert** |
| `tools/bench/gen_flash_own.py` | Generator for flash_own.cfg (reads own_fw.bin + dump) | ✅ canonical |
| `tools/bench/pack_own.py`, `app_crc.py` | Image packing + CRC recompute | ✅ canonical |
| `tools/bench/recovery.cfg` + `recovery_loop.sh` | Catch-and-hold recovery from a brick | ⚠️ **PARTIALLY BROKEN** — carries the `[expr]` watchdog-freeze form (fails on openocd 0.10) AND only heals the bank1 payload, not a page1 brick. Before real use — fix it (direct `mww 0xE0042004 0x00000300`). We keep it as the only recovery mechanism. |
| `archive/old-flashers/flash_revert_page1.cfg` | (old revert) | ❌ BROKEN ([expr]) → archived |
| `archive/old-flashers/flash_revert_full.cfg` | (old revert) | ❌ BROKEN ([expr]) → archived |
| `archive/old-flashers/flash_bank1_*.cfg`, `flash_a3_test.cfg`, `flash_proof.cfg` | bank1 de-risk PoC | ✅ served their purpose → archived |

⚠️ **Known bug of all `[expr]` cfg:** `mww 0xE0042004 [expr {$cr(0)|0x300}]` fails with a usage error on openocd 0.10 BEFORE erase → watchdog not frozen → IWDG reset mid-flash → brick. Always use a direct `mww 0xE0042004 0x00000300`.
