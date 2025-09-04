#include "../include/netinet/udp.h"

void init_udp_header(udp_header *udp, uint16_t len, uint16_t check, uint16_t src_port, uint16_t dst_port)
{
	udp->source = htons(src_port);
	udp->dest= htons(dst_port);
	udp->len = htons(len);
	udp->check = check;
}