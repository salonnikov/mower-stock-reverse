#include "mx_executor.h"

#include "mx_crc8.h"

void mx_executor_init(mx_executor_t *e, const mx_vendor_ops_t *ops,
                      const mx_io_ops_t *io)
{
    e->ops = ops;
    e->io  = io;
    mx_proto_rx_init(&e->rx);
    mx_mixer_init(&e->mixer);
    mx_safety_init(&e->safety);
    e->safin.lift        = false;
    e->safin.hit         = false;
    e->safin.now_ms      = 0U;
    e->safin.last_cmd_ms = 0U;
    e->cmd_l             = 0;
    e->cmd_r             = 0;
}

void mx_executor_send_telemetry(mx_executor_t *e)
{
    mx_telemetry_t t;
    e->ops->read_telemetry(&t);

    uint8_t pl[22];
    size_t  k = 0U;
    pl[k++] = t.state;
    mx_wr_i16(&pl[k], (int16_t)(t.odom_l & 0xFFFF)); k += 2U;
    mx_wr_i16(&pl[k], (int16_t)((t.odom_l >> 16) & 0xFFFF)); k += 2U;
    mx_wr_i16(&pl[k], (int16_t)(t.odom_r & 0xFFFF)); k += 2U;
    mx_wr_i16(&pl[k], (int16_t)((t.odom_r >> 16) & 0xFFFF)); k += 2U;
    mx_wr_i16(&pl[k], t.coil_l);  k += 2U;
    mx_wr_i16(&pl[k], t.coil_r);  k += 2U;
    mx_wr_i16(&pl[k], t.heading); k += 2U;
    mx_wr_u16(&pl[k], t.batt_mv); k += 2U;
    mx_wr_i16(&pl[k], t.cur_l);   k += 2U;
    mx_wr_i16(&pl[k], t.cur_r);   k += 2U;
    pl[k++] = t.flags;

    uint8_t frame[32];
    int n = mx_proto_encode(MX_MSG_TELEMETRY, pl, (uint8_t)k, frame, sizeof frame);
    if (n > 0) {
        e->io->tx(frame, (size_t)n);
    }
}

static void send_status(mx_executor_t *e, uint8_t msg)
{
    uint8_t frame[8];
    int n = mx_proto_encode(msg, NULL, 0U, frame, sizeof frame);
    if (n > 0) {
        e->io->tx(frame, (size_t)n);
    }
}

static void handle_frame(mx_executor_t *e, const mx_frame_t *f)
{
    switch (f->cmd) {
    case MX_CMD_DRIVE:
        if (f->len == 4U) {
            e->cmd_l = mx_rd_i16(&f->data[0]);
            e->cmd_r = mx_rd_i16(&f->data[2]);
            mx_safety_note_cmd(&e->safin, e->ops->now_ms());
            send_status(e, MX_MSG_ACK);
        } else {
            send_status(e, MX_MSG_NACK);
        }
        break;

    case MX_CMD_BLADE:
        if (f->len == 3U) {
            e->ops->blade(f->data[0] != 0U, mx_rd_u16(&f->data[1]));
            send_status(e, MX_MSG_ACK);
        } else {
            send_status(e, MX_MSG_NACK);
        }
        break;

    case MX_CMD_ESTOP:
        e->cmd_l = 0;
        e->cmd_r = 0;
        e->ops->stop();
        send_status(e, MX_MSG_ACK);
        break;

    case MX_CMD_PING:
    case MX_CMD_GET_TELEM:
        mx_executor_send_telemetry(e);
        break;

    default:
        send_status(e, MX_MSG_NACK);
        break;
    }
}

void mx_executor_on_byte(mx_executor_t *e, uint8_t byte)
{
    mx_frame_t f;
    if (mx_proto_rx_feed(&e->rx, byte, &f) == MX_RX_FRAME) {
        handle_frame(e, &f);
    }
}

void mx_executor_tick(mx_executor_t *e)
{
    /* Обновить датчики безопасности из телеметрии. */
    mx_telemetry_t t;
    e->ops->read_telemetry(&t);
    e->safin.lift   = (t.flags & 0x01U) != 0U;
    e->safin.hit    = (t.flags & 0x02U) != 0U;
    e->safin.now_ms = e->ops->now_ms();

    int16_t out_l = 0;
    int16_t out_r = 0;
    if (mx_safety_motion_allowed(&e->safety, &e->safin)) {
        mx_mixer_step(&e->mixer, e->cmd_l, e->cmd_r, &out_l, &out_r);
    } else {
        /* Гарантированно сбрасываем и сглаживание, чтобы не «прыгнуть» при
         * снятии стопа. */
        mx_mixer_step(&e->mixer, 0, 0, &out_l, &out_r);
    }
    e->ops->drive(out_l, out_r);
    e->ops->feed_watchdog();
}
