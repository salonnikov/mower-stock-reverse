#include "mx_crc8.h"

uint8_t mx_crc8_update(uint8_t crc, const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8U; bit++) {
            if ((crc & 1U) != 0U) {
                crc = (uint8_t)((crc >> 1) ^ 0x8CU);
            } else {
                crc = (uint8_t)(crc >> 1);
            }
        }
    }
    return crc;
}

uint8_t mx_crc8(const uint8_t *data, size_t len)
{
    return mx_crc8_update(0U, data, len);
}
