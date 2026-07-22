/*
 * mx_crc8.h — CRC-8 Dallas/Maxim (poly 0x8C reflected, init 0x00).
 *
 * Совместим бит-в-бит с родной прошивкой мейнборда (FUN_080241d0):
 *   init=0; for each byte: crc ^= byte; повторить 8 раз:
 *     crc = (crc & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
 * Используется как для нашего внешнего линка, так и совместимо с шинным
 * фреймингом мейнборда &{...}<crc>#.
 */
#ifndef MX_CRC8_H
#define MX_CRC8_H

#include <stddef.h>
#include <stdint.h>

/* CRC-8 по буферу, начиная с init (0 для нового расчёта). */
uint8_t mx_crc8_update(uint8_t crc, const uint8_t *data, size_t len);

/* Удобная обёртка: полный CRC-8 буфера с init=0. */
uint8_t mx_crc8(const uint8_t *data, size_t len);

#endif /* MX_CRC8_H */
