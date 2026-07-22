#include "mx_proto.h"
#include "mx_crc8.h"

void mx_proto_rx_init(mx_rx_t *rx)
{
    rx->state = MX_ST_SOF;
    rx->len   = 0U;
    rx->cmd   = 0U;
    rx->idx   = 0U;
    rx->crc   = 0U;
}

mx_rx_result_t mx_proto_rx_feed(mx_rx_t *rx, uint8_t byte, mx_frame_t *out)
{
    switch (rx->state) {
    case MX_ST_SOF:
        if (byte == MX_PROTO_SOF) {
            rx->state = MX_ST_LEN;
        }
        return MX_RX_NONE;

    case MX_ST_LEN:
        if (byte > MX_PROTO_MAX_PAYLOAD) {
            mx_proto_rx_init(rx); /* защита от мусора / рассинхрона */
            return MX_RX_ERR;
        }
        rx->len   = byte;
        rx->crc   = mx_crc8_update(0U, &byte, 1U);
        rx->idx   = 0U;
        rx->state = MX_ST_CMD;
        return MX_RX_NONE;

    case MX_ST_CMD:
        rx->cmd   = byte;
        rx->crc   = mx_crc8_update(rx->crc, &byte, 1U);
        rx->state = (rx->len == 0U) ? MX_ST_CRC : MX_ST_PAYLOAD;
        return MX_RX_NONE;

    case MX_ST_PAYLOAD:
        rx->data[rx->idx] = byte;
        rx->crc           = mx_crc8_update(rx->crc, &byte, 1U);
        rx->idx++;
        if (rx->idx >= rx->len) {
            rx->state = MX_ST_CRC;
        }
        return MX_RX_NONE;

    case MX_ST_CRC:
        if (byte != rx->crc) {
            mx_proto_rx_init(rx);
            return MX_RX_ERR;
        }
        rx->state = MX_ST_EOF;
        return MX_RX_NONE;

    case MX_ST_EOF:
    default:
        if (byte == MX_PROTO_EOF) {
            out->cmd = rx->cmd;
            out->len = rx->len;
            for (uint8_t i = 0U; i < rx->len; i++) {
                out->data[i] = rx->data[i];
            }
            mx_proto_rx_init(rx);
            return MX_RX_FRAME;
        }
        mx_proto_rx_init(rx);
        return MX_RX_ERR;
    }
}

int mx_proto_encode(uint8_t cmd, const uint8_t *payload, uint8_t len,
                    uint8_t *out, size_t cap)
{
    if (len > MX_PROTO_MAX_PAYLOAD) {
        return -1;
    }
    const size_t need = (size_t)len + 5U; /* SOF LEN CMD payload CRC EOF */
    if (cap < need) {
        return -1;
    }

    uint8_t crc = 0U;
    size_t  n   = 0U;

    out[n++] = MX_PROTO_SOF;
    out[n++] = len;
    crc      = mx_crc8_update(0U, &len, 1U);
    out[n++] = cmd;
    crc      = mx_crc8_update(crc, &cmd, 1U);
    for (uint8_t i = 0U; i < len; i++) {
        out[n++] = payload[i];
    }
    if (len > 0U) {
        crc = mx_crc8_update(crc, payload, len);
    }
    out[n++] = crc;
    out[n++] = MX_PROTO_EOF;

    return (int)n;
}
