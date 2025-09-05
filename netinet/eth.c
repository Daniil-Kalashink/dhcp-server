#include "../include/netinet/eth.h"

void init_eth_header(eth_header *eth, uint8_t *src_mac, uint8_t *dst_mac, uint16_t eth_type)
{
	if (dst_mac == NULL || src_mac == NULL)
	{
		dhcp_log_error("init_eth_header:Invalid parametr");
		return;
	}

	memcpy(eth->ether_dhost, dst_mac, 6);
	memcpy(eth->ether_shost, src_mac, 6);
	eth->ether_type = htons(eth_type);
}