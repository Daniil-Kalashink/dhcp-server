#ifndef UDP_H
#define UDP_H

#include "../dhcp_common.h"

typedef struct {
    uint16_t source;    // Порт источника
    uint16_t dest;      // Порт назначения
    uint16_t len;       // Длина UDP заголовка + данные
    uint16_t check;     // Контрольная сумма
} udp_header;

void init_udp_header(udp_header *udp, uint16_t len, uint16_t check, uint16_t src_port, uint16_t dst_port);
#endif /*UDP_H*/