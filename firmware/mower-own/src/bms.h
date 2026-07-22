/*
 * bms.h — smart-battery (BMS/pack) link over USART2 for mower-own (GD32F305).
 *
 * The pack keeps its high-current discharge FET (the A4963 motor supply / VBB)
 * enabled only while the host polls it over USART2 (19200 8N1, PD8/PD9). Without
 * this poll the pack opens the FET and the motors are unpowered. This module
 * brings up USART2 and periodically sends the factory poll frame to keep VBB up.
 *
 * Frame + connect bytes were recovered by reverse-engineering the factory image
 * (reverse-v2 BMS spec). v1 is TX-only (the poll is what the pack needs to see);
 * the reply is not parsed — success is observed by the A4963s coming alive.
 */
#ifndef BMS_H
#define BMS_H

#include <stdint.h>

/* Bring up USART2 to the pack (19200 8N1, PD8=TX/PD9=RX, full remap). Call after
 * clocks + hal_gpio_init (GPIOD/AFIO clocks on), before A4963 config so the pack
 * can enable motor VBB before we talk to the drivers. */
void bms_init(void);

/* Send one poll frame to the pack (bounded TX). Sending it is what keeps the
 * discharge FET / motor VBB enabled. Safe to call repeatedly. */
void bms_poll(void);

/* Call once per main loop. Re-sends the poll every BMS_POLL_PERIOD calls so the
 * pack never times the host out and drops VBB. */
void bms_tick(void);

/* Rolling count of polls sent (low 8 bits) — published for SWD/host visibility
 * so we can confirm the link is being serviced. */
uint8_t bms_status(void);

/* Send factory mode/state commands B0..B4 once (test if any enables the pack's
 * motor-discharge / wheel 20U rail). Called at init; SWD-triggerable via pad0. */
void bms_send_bcmds(void);

#endif /* BMS_H */
