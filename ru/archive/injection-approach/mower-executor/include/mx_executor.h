/*
 * mx_executor.h — ядро тонкого исполнителя (Вариант 1).
 *
 * Развязан от железа через таблицы операций:
 *   mx_vendor_ops_t — привод/датчики/watchdog/время (на ARM = обёртки родного
 *                     драйвера; в тестах = моки);
 *   mx_io_ops_t     — передача байт наружу (UART).
 * За счёт этого вся логика (разбор команд, safety, микшер, телеметрия)
 * тестируется на хосте. Динамической памяти нет.
 */
#ifndef MX_EXECUTOR_H
#define MX_EXECUTOR_H

#include <stddef.h>
#include <stdint.h>

#include "mx_mixer.h"
#include "mx_proto.h"
#include "mx_safety.h"
#include "mx_vendor.h"

typedef struct {
    void (*drive)(int16_t left, int16_t right);
    void (*blade)(bool on, uint16_t rpm);
    void (*stop)(void);
    void (*read_telemetry)(mx_telemetry_t *t);
    void (*feed_watchdog)(void);
    uint32_t (*now_ms)(void);
} mx_vendor_ops_t;

typedef struct {
    void (*tx)(const uint8_t *data, size_t len);
} mx_io_ops_t;

typedef struct {
    const mx_vendor_ops_t *ops;
    const mx_io_ops_t     *io;
    mx_rx_t                rx;
    mx_mixer_t             mixer;
    mx_safety_t            safety;
    mx_safety_in_t         safin;
    int16_t                cmd_l; /* последняя запрошенная скорость */
    int16_t                cmd_r;
} mx_executor_t;

void mx_executor_init(mx_executor_t *e, const mx_vendor_ops_t *ops,
                      const mx_io_ops_t *io);

/* Подать входной байт (из UART). Обрабатывает собранные кадры. */
void mx_executor_on_byte(mx_executor_t *e, uint8_t byte);

/* Периодический тик (напр. каждые 20 мс): safety+микшер+привод+watchdog. */
void mx_executor_tick(mx_executor_t *e);

/* Отправить снимок телеметрии наружу (используется PING/GET_TELEM). */
void mx_executor_send_telemetry(mx_executor_t *e);

#endif /* MX_EXECUTOR_H */
