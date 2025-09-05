#ifndef ETH_H
#define ETH_H

#include "../dhcp_common.h"

typedef struct ether_header eth_header;

void init_eth_header(eth_header *eth, uint8_t *src_mac, uint8_t *dst_mac, uint16_t eth_type);

#endif /*ETH_H*/