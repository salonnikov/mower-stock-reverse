/*
 * bms.c — ТОЧНАЯ репликация заводского USART2 пак-линка для mower-own (GD32F305).
 *
 * Не изобретаем — повторяем завод (reverse-v2/factory-map/05-bms-pack.md):
 *   транспорт : USART2 19200 8N1, PD8=TX / PD9=RX (AFIO full remap).
 *   старт     : CE-wake -> connect C1 x4 (при неудаче CE-wake повтор).
 *   рабочий   : НЕПРЕРЫВНЫЙ keep-alive — циклом C1 / C3 / 0x53; при сбое -> CE.
 *   все кадры : из заводской .data, CRC-8/MAXIM проверены байт-в-байт.
 * Ответ пака = кадр с заголовком 3A A3.
 *
 * ПОЧЕМУ ВАЖНО: пак держит разрядный FET (питание мотор-VBB) только пока хост
 * его опрашивает; завод шлёт CE + C1/C3/0x53 постоянно. Наша прежняя версия слала
 * только разовый C1 — это НЕ то, что делает завод. Здесь — как у завода.
 *
 * Диагностика (читать по SWD через символы из build/app.map):
 *   g_bms_polls   — всего обменов; g_bms_replies — сколько увидели 3A A3;
 *   g_bms_connected — 1 если линк живой; g_bms_resp[8]/len — последний ответ;
 *   g_bms_last_op — последний посланный opcode.
 */
#include "bms.h"
#include "gd32_regs.h"

/* Заводские кадры запросов (из .data, CRC-8/MAXIM верны). */
static const uint8_t F_CE[12] =
    { 0x1C,0xA1,0x09,0xCE,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x6E };  /* link-wake */
static const uint8_t F_C1[6]  = { 0x1C,0xA1,0x03,0xC1,0x01,0x2E };   /* connect/telemetry */
static const uint8_t F_C3[6]  = { 0x1C,0xA1,0x03,0xC3,0x01,0xBF };   /* pack message */
static const uint8_t F_53[8]  = { 0x1C,0xA1,0x05,0x53,0x00,0x02,0x1A,0x22 }; /* cell message */

/* Factory "mode/state" commands B0..B4 (CRC-verified). The factory sends these
 * from its FSM (into/exit charge, state); we have NEVER sent them. One of them
 * may switch the pack into motor-discharge mode (enable the wheel 20U tap). */
static const uint8_t F_B0[6]  = { 0x1C,0xA1,0x03,0xB0,0x11,0xC1 };
static const uint8_t F_B1[6]  = { 0x1C,0xA1,0x03,0xB1,0x00,0xC6 };
static const uint8_t F_B2[6]  = { 0x1C,0xA1,0x03,0xB2,0x00,0x93 };
static const uint8_t F_B3[6]  = { 0x1C,0xA1,0x03,0xB3,0x55,0xB3 };
static const uint8_t F_B4[6]  = { 0x1C,0xA1,0x03,0xB4,0x0F,0x78 };

#define BMS_POLL_PERIOD  100U     /* keep-alive каждые ~100 итераций (~100 мс) */
#define BMS_TX_TIMEOUT   20000U   /* bounded TBE/TC ожидание                   */
#define BMS_RX_IDLE      1000000U /* ~60мс: ждём ответ пака (у него задержка в мс) */

/* Диагностика (по SWD через app.map). */
volatile uint16_t g_bms_polls;
volatile uint16_t g_bms_replies;
volatile uint8_t  g_bms_connected;
volatile uint8_t  g_bms_resp[8];
volatile uint8_t  g_bms_resp_len;
volatile uint8_t  g_bms_last_op;

static void tx_byte(uint8_t b)
{
    uint32_t t = BMS_TX_TIMEOUT;
    while ((USART_REG(USART2_BASE, USART_STAT_OFF) & USART_STAT_TBE) == 0U) {
        if (--t == 0U) { return; }
    }
    USART_REG(USART2_BASE, USART_DATA_OFF) = b;
}

/* Один обмен: послать кадр, дождаться TC, вычитать ответ до простоя.
 * Возвращает 1, если в потоке встретился заголовок ответа 3A A3. Bounded. */
static int bms_exchange(const uint8_t *req, unsigned len)
{
    /* HALF-DUPLEX (как заводской FUN_080211f0): линия ОДНА (PD8/PD9 закорочены).
     * На ПЕРЕДАЧУ драйвим PD8 как USART-TX (AF). */
    GPIO_SET_CFG(BMS_TX_PORT, BMS_TX_PIN, GPIO_CFG_AF_PP_50);

    /* Слить устаревшие RX-байты, чтобы парсить свежий ответ. */
    while (USART_REG(USART2_BASE, USART_STAT_OFF) & USART_STAT_RBNE) {
        (void)USART_REG(USART2_BASE, USART_DATA_OFF);
    }

    g_bms_last_op = req[3];
    for (unsigned i = 0; i < len; i++) {
        tx_byte(req[i]);
    }
    uint32_t t = BMS_TX_TIMEOUT;
    while ((USART_REG(USART2_BASE, USART_STAT_OFF) & USART_STAT_TC) == 0U) {
        if (--t == 0U) { break; }
    }

    /* HALF-DUPLEX: ОТПУСКАЕМ PD8 (вход), чтобы пак мог драйвить общий провод, а
     * PD9 (USART-RX) его читал. БЕЗ этого наш TX-пин давил линию и мы слышали
     * только собственное эхо (Test C/D: replies=0). Это и есть заводской свитч. */
    GPIO_SET_CFG(BMS_TX_PORT, BMS_TX_PIN, GPIO_CFG_IN_FLOAT);
    /* Слить эхо своей передачи, которое USART-RX мог захватить, пока PD8 драйвил. */
    while (USART_REG(USART2_BASE, USART_STAT_OFF) & USART_STAT_RBNE) {
        (void)USART_REG(USART2_BASE, USART_DATA_OFF);
    }

    uint8_t buf[16];
    unsigned n = 0;
    uint32_t idle = BMS_RX_IDLE;
    while (n < sizeof buf && idle != 0U) {
        if (USART_REG(USART2_BASE, USART_STAT_OFF) & USART_STAT_RBNE) {
            buf[n++] = (uint8_t)USART_REG(USART2_BASE, USART_DATA_OFF);
            idle = BMS_RX_IDLE;
        } else {
            idle--;
        }
    }

    for (unsigned i = 0; i < sizeof g_bms_resp; i++) {
        g_bms_resp[i] = (i < n) ? buf[i] : 0U;
    }
    g_bms_resp_len = (uint8_t)n;

    for (unsigned i = 0; i + 1U < n; i++) {
        if (buf[i] == 0x3AU && buf[i + 1U] == 0xA3U) {
            g_bms_replies++;
            return 1;
        }
    }
    return 0;
}

void bms_init(void)
{
    RCU_APB1EN |= RCU_APB1EN_USART2EN;
    (void)RCU_APB1EN;

    AFIO_PCF0 = (AFIO_PCF0 & ~AFIO_PCF0_USART2_FULLREMAP)
              | AFIO_PCF0_USART2_FULLREMAP;

    /* HALF-DUPLEX via GPIO direction switching (exactly like factory FUN_080211f0),
     * NOT hardware HDSEL. PD8=USART2 TX, PD9=USART2 RX; the pack is on ONE wire that
     * shorts PD8/PD9. Full-duplex USART (no HDSEL); we RELEASE PD8 (make it input)
     * during RX in bms_exchange so the pack drives the wire and PD9 reads it. PD8
     * starts as AF-TX. */
    GPIO_SET_CFG(BMS_TX_PORT, BMS_TX_PIN, GPIO_CFG_AF_PP_50);   /* PD8 = TX (released for RX) */
    GPIO_SET_CFG(BMS_RX_PORT, BMS_RX_PIN, GPIO_CFG_IN_FLOAT);   /* PD9 = RX (reads wire)      */

    USART_REG(USART2_BASE, USART_CTL1_OFF) = 0U;
    USART_REG(USART2_BASE, USART_CTL2_OFF) = 0U;                /* no HDSEL — GPIO-switched half-duplex */
    USART_REG(USART2_BASE, USART_BAUD_OFF) = USART2_BAUD_19200;
    USART_REG(USART2_BASE, USART_CTL0_OFF) =
        USART_CTL0_UEN | USART_CTL0_TEN | USART_CTL0_REN;

    /* Заводской bring-up: разбудить линк, затем connect C1 x4 (CE при неудаче). */
    (void)bms_exchange(F_CE, sizeof F_CE);
    for (int i = 0; i < 4; i++) {
        if (bms_exchange(F_C1, sizeof F_C1)) {
            g_bms_connected = 1U;
        } else {
            (void)bms_exchange(F_CE, sizeof F_CE);
        }
    }
    /* CLEAN FULLMATCH (2026-07-09 PM): do NOT auto-send the non-factory B0..B4
     * mode commands at init — they tested NEGATIVE and may corrupt pack state.
     * Keep-alive (bms_tick: C1/C3/53) mirrors the factory; the B-command path
     * stays available on demand via MAILBOX->pad0 == 0xB0. */
    /* bms_send_bcmds();  -- disabled for the clean GPIO-fullmatch drive test */
}

void bms_poll(void)
{
    g_bms_polls++;
    if (bms_exchange(F_C1, sizeof F_C1)) { g_bms_connected = 1U; }
}

void bms_tick(void)
{
    static uint32_t ctr;
    static uint8_t  ph;
    if (++ctr < BMS_POLL_PERIOD) { return; }
    ctr = 0;
    g_bms_polls++;

    const uint8_t *f; unsigned n;
    switch (ph++ % 3U) {
    case 0:  f = F_C1; n = sizeof F_C1; break;
    case 1:  f = F_C3; n = sizeof F_C3; break;
    default: f = F_53; n = sizeof F_53; break;
    }
    if (bms_exchange(f, n)) {
        g_bms_connected = 1U;
    } else {
        g_bms_connected = 0U;
        (void)bms_exchange(F_CE, sizeof F_CE);   /* потеря кадра -> re-wake, как завод */
    }
}

/* Send the factory mode/state command set B0..B4 once (test if any enables the
 * pack's motor-discharge / wheel 20U rail). SWD-triggerable via MAILBOX->pad0. */
void bms_send_bcmds(void)
{
    (void)bms_exchange(F_B0, sizeof F_B0);
    (void)bms_exchange(F_B1, sizeof F_B1);
    (void)bms_exchange(F_B2, sizeof F_B2);
    (void)bms_exchange(F_B3, sizeof F_B3);
    (void)bms_exchange(F_B4, sizeof F_B4);
}

uint8_t bms_status(void)
{
    return (uint8_t)((g_bms_connected ? 0x80U : 0U) | (g_bms_polls & 0x7FU));
}
