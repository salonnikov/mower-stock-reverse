/*
 * mx_proto.h — внешний протокол управления (RPi/ESP32 <-> chip1).
 *
 * Компактный бинарный кадр с CRC-8 (совместимым с шиной мейнборда):
 *
 *   SOF(0x26 '&') | LEN | CMD | PAYLOAD[LEN] | CRC8 | EOF(0x23 '#')
 *
 *   LEN  — число байт payload (0..MX_PROTO_MAX_PAYLOAD), CMD не входит в LEN.
 *   CRC8 — по байтам { LEN, CMD, PAYLOAD[0..LEN-1] } (Dallas/Maxim, poly 0x8C).
 *
 * Приёмник — байтовый конечный автомат без динамической памяти (для ISR/таска).
 * Всё портируемо и покрыто хостовыми тестами (см. tests/).
 */
#ifndef MX_PROTO_H
#define MX_PROTO_H

#include <stddef.h>
#include <stdint.h>

#define MX_PROTO_SOF 0x26U /* '&' */
#define MX_PROTO_EOF 0x23U /* '#' */
#define MX_PROTO_MAX_PAYLOAD 64U

/* Команды host -> mower */
enum {
    MX_CMD_PING      = 0x01U, /* payload: нет           -> ответ TELEMETRY */
    MX_CMD_DRIVE     = 0x10U, /* payload: i16 left, i16 right (LE), ед. привода 0..6141 знаковые */
    MX_CMD_BLADE     = 0x11U, /* payload: u8 on, u16 rpm (LE) */
    MX_CMD_ESTOP     = 0x1FU, /* payload: нет — немедленный стоп всего */
    MX_CMD_GET_TELEM = 0x20U  /* payload: нет */
};

/* Ответы mower -> host */
enum {
    MX_MSG_TELEMETRY = 0x80U,
    MX_MSG_ACK       = 0x8EU,
    MX_MSG_NACK      = 0x8FU
};

/* Разобранный кадр */
typedef struct {
    uint8_t cmd;
    uint8_t len;                             /* длина payload */
    uint8_t data[MX_PROTO_MAX_PAYLOAD];
} mx_frame_t;

/* Результат подачи байта */
typedef enum {
    MX_RX_NONE = 0, /* кадр ещё не собран */
    MX_RX_FRAME,    /* кадр готов в out */
    MX_RX_ERR       /* сбой кадрирования/CRC — автомат сброшен */
} mx_rx_result_t;

/* Состояние приёмника */
typedef enum {
    MX_ST_SOF = 0,
    MX_ST_LEN,
    MX_ST_CMD,
    MX_ST_PAYLOAD,
    MX_ST_CRC,
    MX_ST_EOF
} mx_rx_state_t;

typedef struct {
    mx_rx_state_t state;
    uint8_t       len;
    uint8_t       cmd;
    uint8_t       idx;
    uint8_t       crc; /* аккумулятор CRC по LEN,CMD,payload */
    uint8_t       data[MX_PROTO_MAX_PAYLOAD];
} mx_rx_t;

void           mx_proto_rx_init(mx_rx_t *rx);
mx_rx_result_t mx_proto_rx_feed(mx_rx_t *rx, uint8_t byte, mx_frame_t *out);

/*
 * Собрать кадр в out (ёмкость cap). Возвращает длину кадра (>0) или -1 при
 * нехватке места / слишком большом payload.
 */
int mx_proto_encode(uint8_t cmd, const uint8_t *payload, uint8_t len,
                    uint8_t *out, size_t cap);

/* payload-хелперы (little-endian) */
static inline int16_t mx_rd_i16(const uint8_t *p)
{
    return (int16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}
static inline uint16_t mx_rd_u16(const uint8_t *p)
{
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}
static inline void mx_wr_i16(uint8_t *p, int16_t v)
{
    p[0] = (uint8_t)((uint16_t)v & 0xFFU);
    p[1] = (uint8_t)(((uint16_t)v >> 8) & 0xFFU);
}
static inline void mx_wr_u16(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v & 0xFFU);
    p[1] = (uint8_t)((v >> 8) & 0xFFU);
}

#endif /* MX_PROTO_H */
