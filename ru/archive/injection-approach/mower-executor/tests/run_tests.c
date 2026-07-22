/*
 * run_tests.c — хостовые юнит-тесты портируемой логики исполнителя.
 * Собирается нативным gcc/clang, гоняется под ASan/UBSan. Без внешних фреймворков.
 */
#include <stdio.h>
#include <string.h>

#include "mx_crc8.h"
#include "mx_mixer.h"
#include "mx_proto.h"
#include "mx_safety.h"

static int g_fail = 0;
static int g_checks = 0;

#define CHECK(cond)                                                            \
    do {                                                                       \
        g_checks++;                                                            \
        if (!(cond)) {                                                         \
            g_fail++;                                                          \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);             \
        }                                                                      \
    } while (0)

static void test_crc8(void)
{
    /* Эталон CRC-8/MAXIM: строка "123456789" -> 0xA1. */
    const uint8_t v[] = "123456789";
    CHECK(mx_crc8(v, 9) == 0xA1U);

    /* Совместимость с родным алгоритмом: пустой буфер -> init. */
    CHECK(mx_crc8(NULL, 0) == 0x00U);

    /* Инкрементальность == разовому расчёту. */
    const uint8_t a[] = {0x10, 0x02, 0xFF, 0x00, 0x7D};
    uint8_t whole = mx_crc8(a, sizeof a);
    uint8_t part  = mx_crc8_update(0U, a, 2U);
    part          = mx_crc8_update(part, a + 2, sizeof a - 2U);
    CHECK(whole == part);
}

static void test_proto_roundtrip(void)
{
    uint8_t buf[80];
    uint8_t pl[4];
    mx_wr_i16(pl, -1500);
    mx_wr_i16(pl + 2, 6141);

    int n = mx_proto_encode(MX_CMD_DRIVE, pl, 4U, buf, sizeof buf);
    CHECK(n == 9); /* SOF LEN CMD 4*pl CRC EOF */

    mx_rx_t rx;
    mx_proto_rx_init(&rx);
    mx_frame_t f = {0};
    mx_rx_result_t r = MX_RX_NONE;
    for (int i = 0; i < n; i++) {
        r = mx_proto_rx_feed(&rx, buf[i], &f);
    }
    CHECK(r == MX_RX_FRAME);
    CHECK(f.cmd == MX_CMD_DRIVE);
    CHECK(f.len == 4U);
    CHECK(mx_rd_i16(f.data) == -1500);
    CHECK(mx_rd_i16(f.data + 2) == 6141);
}

static void test_proto_zero_payload(void)
{
    uint8_t buf[16];
    int n = mx_proto_encode(MX_CMD_ESTOP, NULL, 0U, buf, sizeof buf);
    CHECK(n == 5);

    mx_rx_t rx;
    mx_proto_rx_init(&rx);
    mx_frame_t f = {0};
    mx_rx_result_t r = MX_RX_NONE;
    for (int i = 0; i < n; i++) {
        r = mx_proto_rx_feed(&rx, buf[i], &f);
    }
    CHECK(r == MX_RX_FRAME);
    CHECK(f.cmd == MX_CMD_ESTOP);
    CHECK(f.len == 0U);
}

static void test_proto_crc_error(void)
{
    uint8_t buf[16];
    int n = mx_proto_encode(MX_CMD_PING, NULL, 0U, buf, sizeof buf);
    buf[n - 2] ^= 0xFFU; /* портим CRC */

    mx_rx_t rx;
    mx_proto_rx_init(&rx);
    mx_frame_t f = {0};
    bool saw_err = false;
    bool saw_frame = false;
    for (int i = 0; i < n; i++) {
        mx_rx_result_t r = mx_proto_rx_feed(&rx, buf[i], &f);
        if (r == MX_RX_ERR) {
            saw_err = true;
        }
        if (r == MX_RX_FRAME) {
            saw_frame = true;
        }
    }
    CHECK(saw_err);       /* битый CRC обязан дать ошибку */
    CHECK(!saw_frame);    /* и не должен собрать кадр */
}

static void test_proto_resync(void)
{
    /* Мусор перед кадром не должен ломать разбор. */
    uint8_t buf[16];
    int n = mx_proto_encode(MX_CMD_GET_TELEM, NULL, 0U, buf, sizeof buf);

    mx_rx_t rx;
    mx_proto_rx_init(&rx);
    mx_frame_t f = {0};
    const uint8_t junk[] = {0x00, 0xFF, 0x23, 0x11};
    for (size_t i = 0; i < sizeof junk; i++) {
        (void)mx_proto_rx_feed(&rx, junk[i], &f);
    }
    mx_rx_result_t r = MX_RX_NONE;
    for (int i = 0; i < n; i++) {
        r = mx_proto_rx_feed(&rx, buf[i], &f);
    }
    CHECK(r == MX_RX_FRAME);
    CHECK(f.cmd == MX_CMD_GET_TELEM);
}

static void test_proto_overlong_rejected(void)
{
    /* LEN > MAX должен дать ошибку и сброс, без выхода за буфер. */
    mx_rx_t rx;
    mx_proto_rx_init(&rx);
    mx_frame_t f = {0};
    (void)mx_proto_rx_feed(&rx, MX_PROTO_SOF, &f);
    mx_rx_result_t r = mx_proto_rx_feed(&rx, (uint8_t)(MX_PROTO_MAX_PAYLOAD + 1U), &f);
    CHECK(r == MX_RX_ERR);
    CHECK(rx.state == MX_ST_SOF);
}

static void test_mixer(void)
{
    CHECK(mx_mixer_clamp(50) == 0);          /* дедбенд */
    CHECK(mx_mixer_clamp(7000) == MX_SPEED_MAX);
    CHECK(mx_mixer_clamp(-7000) == -MX_SPEED_MAX);
    CHECK(mx_mixer_clamp(200) == 200);

    mx_mixer_t m;
    mx_mixer_init(&m);
    int16_t l, r;
    /* слю-рейт: сразу не прыгнет к максимуму */
    mx_mixer_step(&m, 6141, 6141, &l, &r);
    CHECK(l == MX_SLEW_MAX_STEP);
    CHECK(r == MX_SLEW_MAX_STEP);
    /* за много шагов дойдёт до цели */
    for (int i = 0; i < 100; i++) {
        mx_mixer_step(&m, 6141, 6141, &l, &r);
    }
    CHECK(l == MX_SPEED_MAX);
    CHECK(r == MX_SPEED_MAX);
    /* танковый разворот: противоположные знаки */
    mx_mixer_init(&m);
    mx_mixer_step(&m, -6141, 6141, &l, &r);
    CHECK(l == -MX_SLEW_MAX_STEP);
    CHECK(r == MX_SLEW_MAX_STEP);
}

static void test_safety_timeout(void)
{
    mx_safety_t s;
    mx_safety_init(&s);
    mx_safety_in_t in = {0};
    in.lift = false;
    in.hit  = false;

    mx_safety_note_cmd(&in, 1000U);
    in.now_ms = 1200U;
    CHECK(mx_safety_motion_allowed(&s, &in) == true); /* свежая команда */

    in.now_ms = 1000U + MX_CMD_TIMEOUT_MS + 1U;
    CHECK(mx_safety_motion_allowed(&s, &in) == false); /* устарела */
}

static void test_safety_estop_latch(void)
{
    mx_safety_t s;
    mx_safety_init(&s);
    mx_safety_in_t in = {0};
    mx_safety_note_cmd(&in, 100U);
    in.now_ms = 150U;

    in.hit = true;
    CHECK(mx_safety_motion_allowed(&s, &in) == false);

    /* даже после снятия hit латч держится, пока не сброшен явно */
    in.hit = false;
    mx_safety_note_cmd(&in, 150U);
    CHECK(mx_safety_motion_allowed(&s, &in) == false);

    mx_safety_clear(&s, &in);
    CHECK(mx_safety_motion_allowed(&s, &in) == true);
}

static void test_safety_clear_blocked_while_active(void)
{
    mx_safety_t s;
    mx_safety_init(&s);
    mx_safety_in_t in = {0};
    mx_safety_note_cmd(&in, 100U);
    in.now_ms = 150U;
    in.lift   = true;
    CHECK(mx_safety_motion_allowed(&s, &in) == false);
    /* сброс при активном lift не должен снять латч */
    mx_safety_clear(&s, &in);
    CHECK(mx_safety_motion_allowed(&s, &in) == false);
}

/* ---- Моки для тестов исполнителя ---- */
#include "mx_executor.h"

static int16_t  mk_drive_l, mk_drive_r;
static int      mk_drive_calls;
static int      mk_stop_calls;
static int      mk_wdt_calls;
static uint32_t mk_now;
static uint8_t  mk_flags;
static uint8_t  mk_tx[256];
static size_t   mk_tx_len;

static void mk_drive(int16_t l, int16_t r) { mk_drive_l = l; mk_drive_r = r; mk_drive_calls++; }
static void mk_blade(bool on, uint16_t rpm) { (void)on; (void)rpm; }
static void mk_stop(void) { mk_stop_calls++; mk_drive_l = 0; mk_drive_r = 0; }
static void mk_read_telem(mx_telemetry_t *t)
{
    t->state = 6; t->odom_l = 0x11223344; t->odom_r = -5;
    t->coil_l = 100; t->coil_r = -100; t->heading = 42; t->batt_mv = 16800;
    t->cur_l = 0; t->cur_r = 0; t->flags = mk_flags;
}
static uint32_t mk_now_ms(void) { return mk_now; }
static void mk_tx_fn(const uint8_t *d, size_t n)
{
    for (size_t i = 0; i < n && mk_tx_len < sizeof mk_tx; i++) { mk_tx[mk_tx_len++] = d[i]; }
}

static const mx_vendor_ops_t MK_OPS = {
    .drive = mk_drive, .blade = mk_blade, .stop = mk_stop,
    .read_telemetry = mk_read_telem, .feed_watchdog = NULL, .now_ms = mk_now_ms
};
static void mk_wdt(void) { mk_wdt_calls++; }
static const mx_io_ops_t MK_IO = { .tx = mk_tx_fn };

static void feed_frame(mx_executor_t *e, uint8_t cmd, const uint8_t *pl, uint8_t len)
{
    uint8_t buf[80];
    int n = mx_proto_encode(cmd, pl, len, buf, sizeof buf);
    for (int i = 0; i < n; i++) { mx_executor_on_byte(e, buf[i]); }
}

static void test_executor_drive_and_tick(void)
{
    mx_vendor_ops_t ops = MK_OPS; ops.feed_watchdog = mk_wdt;
    mx_executor_t e;
    mx_executor_init(&e, &ops, &MK_IO);
    mk_drive_calls = 0; mk_wdt_calls = 0; mk_flags = 0; mk_tx_len = 0;

    mk_now = 1000U;
    uint8_t pl[4]; mx_wr_i16(pl, 2000); mx_wr_i16(pl + 2, -2000);
    feed_frame(&e, MX_CMD_DRIVE, pl, 4U);
    CHECK(e.cmd_l == 2000);
    CHECK(e.cmd_r == -2000);

    mk_now = 1010U;
    mx_executor_tick(&e);
    CHECK(mk_drive_calls == 1);
    CHECK(mk_wdt_calls == 1);
    /* слю-рейт: первый шаг ограничен MX_SLEW_MAX_STEP */
    CHECK(mk_drive_l == MX_SLEW_MAX_STEP);
    CHECK(mk_drive_r == -MX_SLEW_MAX_STEP);
}

static void test_executor_failsafe(void)
{
    mx_vendor_ops_t ops = MK_OPS; ops.feed_watchdog = mk_wdt;
    mx_executor_t e;
    mx_executor_init(&e, &ops, &MK_IO);
    mk_flags = 0;

    mk_now = 1000U;
    uint8_t pl[4]; mx_wr_i16(pl, 5000); mx_wr_i16(pl + 2, 5000);
    feed_frame(&e, MX_CMD_DRIVE, pl, 4U);

    /* Команда устарела -> движение запрещено -> привод 0 */
    mk_now = 1000U + MX_CMD_TIMEOUT_MS + 100U;
    mx_executor_tick(&e);
    CHECK(mk_drive_l == 0);
    CHECK(mk_drive_r == 0);
}

static void test_executor_estop_on_hit(void)
{
    mx_vendor_ops_t ops = MK_OPS; ops.feed_watchdog = mk_wdt;
    mx_executor_t e;
    mx_executor_init(&e, &ops, &MK_IO);

    mk_now = 1000U;
    uint8_t pl[4]; mx_wr_i16(pl, 3000); mx_wr_i16(pl + 2, 3000);
    feed_frame(&e, MX_CMD_DRIVE, pl, 4U);

    mk_flags = 0x02U; /* hit */
    mk_now = 1010U;
    mx_executor_tick(&e);
    CHECK(mk_drive_l == 0);
    CHECK(mk_drive_r == 0);
}

static void test_executor_telemetry_frame(void)
{
    mx_executor_t e;
    mx_executor_init(&e, &MK_OPS, &MK_IO);
    mk_flags = 0x05U; mk_tx_len = 0;

    feed_frame(&e, MX_CMD_PING, NULL, 0U);
    /* декодируем то, что ушло в tx, и проверяем поля */
    mx_rx_t rx; mx_proto_rx_init(&rx);
    mx_frame_t f = {0};
    mx_rx_result_t r = MX_RX_NONE;
    for (size_t i = 0; i < mk_tx_len; i++) { r = mx_proto_rx_feed(&rx, mk_tx[i], &f); }
    CHECK(r == MX_RX_FRAME);
    CHECK(f.cmd == MX_MSG_TELEMETRY);
    CHECK(f.data[0] == 6);                 /* state */
    CHECK(f.data[f.len - 1] == 0x05U);     /* flags */
}

static void test_executor_estop_cmd(void)
{
    mx_executor_t e;
    mx_executor_init(&e, &MK_OPS, &MK_IO);
    mk_stop_calls = 0;
    feed_frame(&e, MX_CMD_ESTOP, NULL, 0U);
    CHECK(mk_stop_calls == 1);
    CHECK(e.cmd_l == 0);
}

int main(void)
{
    test_crc8();
    test_proto_roundtrip();
    test_proto_zero_payload();
    test_proto_crc_error();
    test_proto_resync();
    test_proto_overlong_rejected();
    test_mixer();
    test_safety_timeout();
    test_safety_estop_latch();
    test_safety_clear_blocked_while_active();
    test_executor_drive_and_tick();
    test_executor_failsafe();
    test_executor_estop_on_hit();
    test_executor_telemetry_frame();
    test_executor_estop_cmd();

    printf("checks=%d failures=%d\n", g_checks, g_fail);
    return g_fail == 0 ? 0 : 1;
}
