# 02 — Power button, PIN code, unlock, display link (chip1, factory)

The "POWER BUTTON + PIN CODE + unlock + display" branch. Reverse-engineered from
`reverse-v2/chip1/decompiled_all.c` (+ disassembly of the dump `dist/gd32-mainboard-dump-v1.bin`
via `objcopy -I binary -O elf32-littlearm` for functions that Ghidra failed to parse).
Everything is in flash `0x08000000`. Date: 2026-07-08.

> **Main conclusion for the wheel investigation:** the entire "button → PIN → unlock" path
> is pure UART + cJSON + NVM. It **does not touch a single power GPIO/rail, nor A4963,
> nor the 20U enable**. See section (5).

---

## (1) FUNCTION MAP

### Power button / latches (src/user/key.c, power service)
| Address | Name | Role |
|---|---|---|
| `FUN_0800d034` | key_read | Reads the button: `FUN_0800cab2(port,0x400)` (mask bit10) with debounce (>9 ticks). Return: 1=press edge, 2=release, 3=idle/hold. |
| `FUN_0800cf50` | key_handler | Based on key_read: when state==3 (hold) and counter>1 sets flag `|0x40`, logs **`key_press_power_on`** (`s_..._0800d01c`); on release — `poweroff`/`key_press_down`. |
| `FUN_08055xxx` (power service) | — | Log tags `s_power_..._08055f78`, `button_driver_get_null`/`not_initialed` (@`0x08055f24`) — the button/power service. |
| latch **PE12** OUT-PP HIGH | MAIN power latch | Set by the **bootloader**, inherited by the app. Self-holds power for the whole board. |
| latch **PB0** OUT-PP HIGH | secondary latch | Also bootloader-inherited. |

### Boot / entry points
reset → `FUN_08019430` (C-runtime) → **main `FUN_08061714`** → `FUN_0807f010` (RCU+flash) →
FreeRTOS → task "initial" → **rw_init `FUN_080477c4`** (drivers). PE12/PB0 are already HIGH by this point.

### Display link (ESP32) — driver_dpport / UART
| Address | Name (reverse) | Role |
|---|---|---|
| `FUN_0804721c` | **dp_receive task** | RX task (task name `"dp receive"`/string `dpport_receive_task`@`0x0808864a`). Loop: receive from queue `FUN_08084...`, `malloc(0x100)`=`FUN_0807162c`, → `FUN_0803953c`. **(MISSING from decompiled_all.c — Ghidra gap 0x0804716c–0x08047470; parsed via disassembly.)** |
| `FUN_0803953c` | dpport_deal_msg | Length check (3<len<0x101). If flag=1 — **de-scramble payload XOR `0x5b`** (bytes 1..len-2). Then → `FUN_0806e938`. |
| `FUN_0806e938` | dpport_frame_decode | Frame validation `&…#`, **CRC-8**, framing strip → clean payload. |
| `FUN_080241d0` / (twin @~`0x08008378`) | crc8 | CRC-8, reflected poly **`0x8C`**, init 0. |
| `FUN_080470f0` | dpport_dispatch | **cJSON parse** (`FUN_08023fe6`), takes key `"cmd"` (`FUN_08023f80(json,&DAT_0804716c)`), dispatch over a 16-entry table: `matcher@+0x60`, `handler@+0x64`, stride 8, key = `cmd & 0xFF000000`. Then `cJSON_Delete`. |
| `FUN_08009d7c` | dpport_frame_wrap (TX) | Builds the outgoing frame: `buf[0]=0x26`, payload, `buf[len+1]=0x23`. |
| `FUN_08072e08` | dpport_reply | Assembles the reply cJSON `{<id>, "result": <bool>}` and sends it to the display (vtable+0x18). |

### Password / PIN code
| Address | Name | Role |
|---|---|---|
| `FUN_08044c80` (size 6676) | **userset service** = handler for cmd class `0x30xxxxxx` | Large switch over the full `cmd` value; PIN entry lands here. |
| `FUN_08060d00` | load_user_password | Reads the password from NVM into `DAT_08060d38` (`DAT_08060d34`=length, 4 bytes). Error → log `load_user_password_failed`. |
| `FUN_0807bf20` | set_password_error_count | Writes the PIN error counter to NVM "run_param", offset `+0xb8`. |
| storage `DAT_08060d38` | user_password | The loaded password. |
| vtable userset (`FUN_08050578`) | — | `+0x16c` get error-count, `+0x170` set count, `+0x17c` lock, `+0x258`(600) get stored pwd. |

### FSM setter
| Address | Name | Role |
|---|---|---|
| `FUN_0807976c` | **fsm_set_state** | `if(state<0xb) *(DAT_08079778+0x10)=state;` — writes the robot's global state. |
| `FUN_08049d14` | fsm_set_state_wrap | `FUN_0804b190(); FUN_0807976c(param);` — a thin wrapper. |

States: **3=idle/unlock, 4=departure, 5=cutting, 6/7=docking variants, 8=battery-low, 9=move/search, 10=docked/charge**.

---

## (2) TIMELINE "button → powered on → PIN → verify → unlock (state 3)"

1. **Button press.** `FUN_0800d034` reads the GPIO input (mask `0x400`=bit10) with debounce;
   `FUN_0800cf50` on hold (state 3) sets flag `|0x40` and logs `key_press_power_on`.
2. **Power self-hold.** The bootloader raised **PE12 HIGH** (main latch) + **PB0 HIGH**
   (secondary) — the board holds its own power. The app inherits this and does not re-drive the latch GPIOs.
3. **App boot.** `FUN_08061714`(main) → RCU/flash → FreeRTOS → `FUN_080477c4`(rw_init):
   drivers come up, including dpport (UART to the display) and userset. The password is loaded via
   `FUN_08060d00 → DAT_08060d38`.
4. **Display (ESP32) requests the PIN.** Exchange over UART using dpport frames (section 3). The user
   enters the PIN on the display.
5. **Display sends the PIN to chip1.** Frame `0x26 | <cJSON {"cmd":0x30000009, "<pwd-key>":<pin>}> | CRC8 | 0x23`.
6. **chip1 receive:** `FUN_0804721c`(dp_receive) → `FUN_0803953c` (with flag — XOR `0x5b`) →
   `FUN_0806e938` (checks `&…#` + CRC-8) → `FUN_080470f0` (cJSON parse, `cmd`, dispatch by
   `cmd&0xFF000000`=`0x30000000` → **`FUN_08044c80`**).
7. **PIN verify** (`FUN_08044c80`, case `0x30000009`):
   - `iVar19 = cJSON_GetObjectItem(json, DAT_080450b4)` — the entered PIN;
   - `iVar9 = vtable[+0x258]()` — the stored password;
   - **match** → `FUN_08072e08(id,0)` (reply to display `result=true`); reset the error counter;
     log `compare_pwd_correct` (`s_..._080450e8`);
   - **no match** → increment the counter (`vtable[+0x170]`); if `>9` → **lock**:
     `vtable[+0x17c](0,cnt)`, `FUN_08050448()[+0x14](0x20)` (event 0x20), log
     `compare_pwd_uncorrect ... overtime`; otherwise log `compare_pwd_uncorrect=%d`, reply `result=false`.
8. **Unlock (state 3).** The PIN path itself does NOT poke the FSM — it only sends `result` to the display and
   clears the security flag. The actual transition into **state 3 (idle/unlock)** is made by the process layer:
   `FUN_08069848` (process_find_bd), line 77890 `FUN_08049d14(3)` — when the signal bit is non-negative
   during start/base search. The log `Robot unlock success` (@`0x0806afc8`)
   is printed by the security service (a function in a Ghidra gap, xref via base+offset, not the literal pool).

> That is, PIN verification is a **request/response** over the display's JSON protocol; the change of
> robot state is separate, in the app FSM, driven by the fact that security has been cleared.

---

## (3) dpport PROTOCOL (frame + opcode table)

### Frame (bytes between chip1 ↔ display)
```
+------+------------------+--------+------+
| 0x26 |  payload[N]      | CRC8   | 0x23 |
| '&'  |  (ASCII cJSON)   | 1 byte | '#'  |
+------+------------------+--------+------+
total length = N+3,  maximum 0x100 (256) bytes
```
- **Header** `0x26` ('&'), **Trailer** `0x23` ('#').
- **CRC8** = `FUN_080241d0`: reflected poly **`0x8C`**, init 0, computed over `payload` (without header/CRC/trailer), placed as a single byte BEFORE `#`.
- **Optional scrambling**: with the mode flag, payload XOR `0x5b` (bytes 1..N) — de-scrambled in `FUN_0803953c`. There is also a mode without CRC/scramble (param_5≠1 in `FUN_0806e938`): then payload = len-2, CRC is not checked.
- `FUN_0806e938` can also search for `&…#` inside a "dirty" buffer (start/stop scan).
- **Payload = cJSON string**, with a required `"cmd"` field (u32). The high byte of `cmd` (`&0xFF000000`) selects the handler in a table of **16 entries** (`matcher@+0x60`, `handler@+0x64`, stride 8). Class `0x30xxxxxx` → `FUN_08044c80` (userset). Other classes (0x10/0x20/0x40…) → other services (bd/battery/map/query); their registration is a static init array, runtime pointer `_DAT_08047170`.

### Display commands of class 0x30 (userset, `FUN_08044c80`)
| cmd | Meaning | Action / log string |
|---|---|---|
| `0x30000005` | (query/reset branch) | `LAB_080459cc` |
| `0x30000006` | **into manual set** | `*(struct+4)=0`, log `into_manual_set` |
| `0x30000007` | **quit manual set** | `*(struct+4)=0xff`, log `quit_manual_set` |
| `0x30000009` | **compare_pwd (PIN entry)** | PIN verify, `result` reply, counter/lock |
| `0x30000010` | **set_pwd** (new password) | `vtable[+0x25c](pwd)`, `set_pwd_success/failed` |
| `0x30000011` | set (u16 param, vtable+0x10) | — |
| `0x30000012`–`0x30000014` | **set_start_time** | schedule start hour, `set_start_time_success hour=%d` |
| `0x30000015` | **set_work_time** | work minutes, `set_work_time %d minutes success` |
| `0x30000016`–`0x3000001d` | settings | various set commands |
| `0x3000001f` | **set_pwd_old** (change password with old one) | `compare_pwd_old_success` / `set_pwd_old_failed because input old password error` |
| `0x30000020`–`0x30000023` | **reset_pwd** | password reset, `reset_pwd_success/failed` |
| default | — | `undefine command=%d` |

### chip1 → display commands (TX)
- Reply to a command: `FUN_08072e08` → cJSON `{<id>, "result": true|false}` (`s_result_08072e8c`), wrapper `FUN_08009d7c`.
- Telemetry/state: periodic sends (a separate service's class), e.g. `send cmd into charge`, `send cmd exit charge`, `error send battery state cmd` (@`0x08072a37`/`0x08072a84`).

---

## (4) KEY CODE (annotated)

### Frame decode + CRC — `FUN_0806e938`
```c
uVar6 = *param_2;                       // len
if (uVar6 < 4) return -1;               // too short
if (*param_1=='&' && param_1[uVar6-1]=='#') {   // 0x26 ... 0x23
  if (param_5 == 1) {                   // CRC mode
    *param_4 = uVar6 - 3;               // payload length
    uVar3 = FUN_080241d0(param_1+1);    // CRC8(payload), poly 0x8C
    if ((byte)param_1[uVar6-2] != uVar3) return -1;   // byte before '#'
    memcpy(param_3, param_1+1, *param_4);             // clean payload
    return 0;
  } else { *param_4=uVar6-2; memcpy(param_3,param_1+1,uVar6-2); return 0; }
}
// otherwise: scan '&' ... '#' inside the buffer (same CRC mechanism)
```

### Opcode dispatch — `FUN_080470f0`
```c
iVar1 = cJSON_Parse(payload);                   // FUN_08023fe6
iVar2 = cJSON_GetObjectItem(iVar1, "cmd");      // FUN_08023f80(&DAT_0804716c)
iVar4 = *(_DAT_08047170 + 4);                   // base of the handler table
for (i=0; i<0x10; i++)                           // 16 entries
  if (*(iVar4 + i*8 + 0x60) == (cmd_val & 0xFF000000)) {  // matcher
     h = *(iVar4 + i*8 + 0x64);                  // handler
     if (h) h(iVar1);                            // call with json
     break;
  }
cJSON_Delete(iVar1);                             // FUN_08023f38
```

### PIN verify — `FUN_08044c80`, case `0x30000009`
```c
iVar19 = cJSON_GetObjectItem(json, DAT_080450b4);   // entered PIN
if (iVar19) {
  iVar9 = vtable[+0x258]();                          // stored password (=600)
  if (*(iVar19+0x14) == iVar9) {                     // MATCH
     FUN_08072e08(id, 0);                            // reply result=true
     if (vtable[+0x16c]() == 0) log("compare_pwd_correct");
     ...
  } else {                                           // NO match
     FUN_08072e08(id, -1);                           // reply result=false
     cnt = (vtable[+0x16c]() + 1) & 0xff;
     if (cnt > 9) { vtable[+0x17c](0,cnt);           // LOCK
                    FUN_08050448()[+0x14](0x20);      // lock event
                    log("compare_pwd_uncorrect overtime"); }
     else { vtable[+0x170](cnt); log("compare_pwd_uncorrect=%d"); }
  }
} else { FUN_08072e08(id,-1); log("compare_pwd_err"); }
```

### FSM setter — `FUN_0807976c`
```c
void FUN_0807976c(uint state){ if(state<0xb) *(uint*)(DAT_08079778+0x10)=state; }
```

### All call sites of FUN_0807976c / FUN_08049d14 and which state they set
| Function | state | Condition |
|---|---|---|
| `FUN_08025be0` | 10 / 9 | dock-check ok → 10; otherwise 9 |
| `FUN_08025c16` | 10 / param | dock-check |
| `FUN_080270a8` (scheduler) | **4** (×3) / 8 | 4 = departure (auto-dispatch/schedule/gate, safety ok); 8 = battery-low |
| `FUN_0802bce4` / `FUN_0803cb54` / `FUN_08049cfc` / `FUN_08083d9c` | 9 | unconditional move/error-fallback |
| `FUN_0802bd10` / `FUN_08032dd8` / `FUN_0803cb78` | param | passthrough |
| `FUN_080391e4` | 8 / 6 / 7 | dock commands (idle/border) |
| `FUN_08054274` | **5** | cutting: counter @+0x42 < 0x40 |
| `FUN_08063c8c` | 9 | zone timeout |
| `FUN_08069848` (find_bd) | 10 / 10 / **3** / 7 / 7 | 3(unlock) @77890 = signal bit not negative; 10 = battery<3 / signal negative; 7 = at the border |

> **state 4 (departure) / 5 (cutting)** — "go mow" — is set ONLY by the scheduler/process layer
> (`FUN_080270a8`, `FUN_08054274`), **not** by the PIN/display path. The display can only influence
> manual-set/schedule/password via userset commands.

---

## (5) LINK TO WHEEL POWER

- **PIN/display/unlock do NOT touch the power rails.** The whole path `FUN_0804721c → FUN_0803953c
  → FUN_0806e938 → FUN_080470f0 → FUN_08044c80 → FUN_08072e08` is UART receive, CRC, cJSON, NVM
  and sending a reply. There is not a single write to a GPIO OCTL/BOP, no access to A4963/SPI1, no 20U enable.
- **Power latches PE12 (main) + PB0 (secondary)** power the WHOLE board, are set by the bootloader,
  and are not drive-specific. The button (`FUN_0800d034`, input mask `0x400`) only detects press/
  hold for power-on/off, it does not touch the latch itself (it is already held).
- **state 3 (unlock) ≠ turning on the wheel rail.** Unlock moves the FSM into idle; movement
  (state 4/5) is initiated by the scheduler. None of these transitions in the disassembled code raises
  the switched 20U rail.
- Consistent with the overall status (`reports/WHEELS-STATE-2026-07-08.md`): the wheel rail enable is **not
  a firmware GPIO and not in the button/PIN/display branch**. This branch is clean of wheel power.
- Noted in passing (for power tracking, do NOT touch): `PE0 HIGH` = "enable event" `FUN_08063368`;
  `PE7 HIGH` = aux rail; USART2/BMS keep-alive (`bms-usart2-spec`) — the only power
  delta candidate, but it is outside this branch.

---
### Notes on method
- The functions `FUN_0804721c` (dp_receive) and the security service (`Robot unlock success`, `security_password_right`)
  are MISSING from `decompiled_all.c` — Ghidra did not carve them out (gaps in function_index). Parsed via
  disassembly of the dump (`objcopy -O elf32-littlearm` + `objdump --disassembler-options=force-thumb`).
- The string `dpport_receive_task`@`0x0808864a` is the task NAME (rodata), not a function address;
  the real RX function = `FUN_0804721c` (pointer from `DAT_08058a3c=0x0804721d`).
