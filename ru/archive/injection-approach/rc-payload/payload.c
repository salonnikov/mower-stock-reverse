/*
 * payload.c — RC-исполнитель chip1 (GD32F305) v2: heartbeat + RPC + RC-привод левого.
 *
 * Зачем RPC: энергизация мотора (перевод A4963 в RUN) статически до конца не разрешена
 * (рантайм-vtable). Чтобы подтвердить БЕЗ гадания и БЕЗ перезаливок — payload умеет по
 * команде из mailbox вызвать ЛЮБУЮ функцию по адресу с 4 аргументами и вернуть результат.
 * Тогда с хоста (SWD) можно вживую звать init/enable/set_left/disable и смотреть, что реально
 * крутит колесо. Найдём рабочую последовательность — зашьём её в финальную версию.
 *
 * Hook: BL FUN_0808368c @0x08053b0e → mx_veneer (аддитивно, родная FSM/watchdog работают).
 *
 * Mailbox 0x20014000:
 *   +0x00 u32 heartbeat   — payload++ каждый тик (проверка hook)
 *   +0x04 u32 magic       — ВЗВОД: всё ниже активно только если == 0x5243414D (иначе только heartbeat)
 *   +0x08 u32 rpc_trigger — хост=1 → payload зовёт rpc_addr(a0..a3) ОДИН раз, кладёт ret, seq++, чистит в 0
 *   +0x0C u32 rpc_addr    — адрес функции (thumb-бит!)
 *   +0x10 u32 rpc_a0
 *   +0x14 u32 rpc_a1
 *   +0x18 u32 rpc_a2
 *   +0x1C u32 rpc_a3
 *   +0x20 u32 rpc_ret     — payload пишет возврат (low32)
 *   +0x24 u32 rpc_seq     — payload инкрементит после каждого выполненного вызова (хост видит факт)
 *   +0x28 u8  rc_active   — 1: каждый тик звать set_left(rc_dir, rc_speed)
 *   +0x29 u8  rc_dir      — 0/1
 *   +0x2C u16 rc_speed    — |скорость| в единицах set_left
 *
 * ⚠ boot-safe: до записи magic хостом payload ТОЛЬКО тикает heartbeat (мусор RAM не крутит моторы).
 * Нет .data/.bss с ненулевой инициализацией.
 */
#include <stdint.h>

#define FUN_PACE     0x0808368dU   /* FUN_0808368c(uint*,int): 10мс-пейсинг [OK] */
#define SET_LEFT     0x0805f621U   /* FUN_0805f620(dir,|speed|): левый, CH3CV [OK] */
#define MAILBOX      0x20014000U
#define MB_MAGIC_VAL 0x5243414DU

typedef uint32_t (*rpc_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);
typedef void     (*drive_fn_t)(int32_t, int32_t);

void mx_tick(void);
void mx_tick(void)
{
    volatile uint32_t *m  = (volatile uint32_t *)MAILBOX;
    volatile uint8_t  *mb = (volatile uint8_t  *)MAILBOX;

    m[0]++;                                     /* heartbeat (всегда, безопасно) */

    if (m[1] != MB_MAGIC_VAL) {                 /* magic @0x04: не взведён → инертны */
        return;
    }

    if (m[2] != 0U) {                           /* rpc_trigger @0x08: одноразовый вызов */
        uint32_t ret = ((rpc_fn_t)m[3])(m[4], m[5], m[6], m[7]); /* addr@0x0C, a0..a3 @0x10-0x1C */
        m[8] = ret;                             /* rpc_ret @0x20 */
        m[9]++;                                 /* rpc_seq @0x24 */
        m[2] = 0U;                              /* clear trigger */
    }

    if (mb[0x28] != 0U) {                        /* rc_active @0x28: непрерывный привод левого */
        uint8_t  dir = mb[0x29];                 /* rc_dir @0x29 */
        uint16_t spd = *(volatile uint16_t *)(MAILBOX + 0x2CU); /* rc_speed @0x2C */
        ((drive_fn_t)SET_LEFT)((int32_t)dir, (int32_t)spd);
    }
}

/* mx_veneer — вход payload (0x08090000). Заменяет `bl FUN_0808368c` в hs-loop. */
__attribute__((naked, used, section(".text.entry"))) void mx_veneer(void);
void mx_veneer(void)
{
    __asm__ volatile(
        "push {lr}          \n"
        "ldr  r2, =0x0808368d\n"   /* thumb-адрес FUN_0808368c */
        "blx  r2            \n"    /* оригинальный пейсинг: r0/r1 от caller целы */
        "bl   mx_tick       \n"
        "pop  {pc}          \n"
    );
}
