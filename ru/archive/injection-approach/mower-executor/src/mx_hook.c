/*
 * mx_hook.c — интеграция блоба с родной прошивкой chip1.
 *
 * Задачи:
 *   1. mx_hook_entry — функция, на которую перенаправляется метод-tick
 *      process-manager (вызывается родным таском периодически). Драйнит UART,
 *      крутит исполнитель.
 *   2. mx_hook_install — записать адрес mx_hook_entry в обёртку сервиса
 *      process-manager (перехват vtable), сохранив оригинал.
 *
 * Весь ввод/вывод и указатели — через mx_vendor.* и адреса mx_vendor.h.
 * Всё [H] помечено; на живом чипе подтверждается перед заливкой.
 */
#include "mx_executor.h"
#include "mx_vendor.h"

/* [H] UART наружу (к RPi/ESP): адреса функций родного драйвера порта. */
#define MX_ADDR_UART_RX_AVAIL 0x00000000U /* int uart_rx_available(void) */
#define MX_ADDR_UART_RX_BYTE  0x00000000U /* int uart_read_byte(void) */
#define MX_ADDR_UART_TX       0x00000000U /* void uart_write(const u8*, u32) */

/* Стейт блоба (в .bss нашего RAM-региона; обнуляется явно в install). */
static mx_executor_t g_exec;
static void         *g_orig_tick; /* сохранённый оригинальный метод */
static int           g_inited;

static void hook_tx(const uint8_t *data, size_t len)
{
    if (MX_ADDR_UART_TX != 0U) {
        void (*tx)(const uint8_t *, uint32_t) =
            (void (*)(const uint8_t *, uint32_t))(uintptr_t)MX_ADDR_UART_TX;
        tx(data, (uint32_t)len);
    }
}

static const mx_vendor_ops_t VENDOR_OPS = {
    .drive          = mx_vendor_drive,
    .blade          = mx_vendor_blade,
    .stop           = mx_vendor_stop,
    .read_telemetry = mx_vendor_read_telemetry,
    .feed_watchdog  = mx_vendor_feed_watchdog,
    .now_ms         = mx_vendor_now_ms,
};
static const mx_io_ops_t IO_OPS = { .tx = hook_tx };

static void drain_uart(void)
{
    if (MX_ADDR_UART_RX_AVAIL == 0U || MX_ADDR_UART_RX_BYTE == 0U) {
        return;
    }
    int (*avail)(void)   = (int (*)(void))(uintptr_t)MX_ADDR_UART_RX_AVAIL;
    int (*readb)(void)   = (int (*)(void))(uintptr_t)MX_ADDR_UART_RX_BYTE;
    unsigned guard       = 0U;
    while (avail() > 0 && guard < 256U) {
        int b = readb();
        if (b < 0) {
            break;
        }
        mx_executor_on_byte(&g_exec, (uint8_t)b);
        guard++;
    }
}

/* Вызывается родным process-manager вместо его tick-метода. */
void mx_hook_entry(void);
void mx_hook_entry(void)
{
    if (g_inited == 0) {
        mx_executor_init(&g_exec, &VENDOR_OPS, &IO_OPS);
        g_inited = 1;
    }
    drain_uart();
    mx_executor_tick(&g_exec);
    /* Родной tick НЕ вызываем: навигацию заменяем целиком. Watchdog кормим сами
     * (mx_executor_tick -> feed_watchdog). g_orig_tick сохранён для отката. */
    (void)g_orig_tick;
}

/*
 * Установить перехват: (*PMGR_SLOT) -> обёртка; обёртка[TICK_INDEX] := entry.
 * Возвращает 0 при успехе, -1 если анкеры не подтверждены (нулевые).
 */
int mx_hook_install(void);
int mx_hook_install(void)
{
    g_inited     = 0;
    g_orig_tick  = (void *)0;

    if (MX_ADDR_PMGR_SLOT == 0U) {
        return -1;
    }
    void **wrapper = *(void ***)(uintptr_t)MX_ADDR_PMGR_SLOT;
    if (wrapper == (void **)0) {
        return -1;
    }
    g_orig_tick = wrapper[MX_PMGR_TICK_INDEX];
    /* thumb-бит на нашей функции */
    wrapper[MX_PMGR_TICK_INDEX] =
        (void *)((uintptr_t)&mx_hook_entry | 1U);
    return 0;
}
