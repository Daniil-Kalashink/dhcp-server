#include "../include/netinet/ipv4.h"

unsigned short checksum(void *b, int len) {
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;

	for (sum = 0; len > 1; len -= 2) {
		sum += *buf++;
	}
	if (len == 1) {
		sum += *(unsigned char *)buf;
	}
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void init_ipv4_header(ip_header *ip, uint16_t id, uint16_t frag_off, uint8_t protocol, uint16_t len,
         struct in_addr *src_ip, struct in_addr *dst_ip, uint8_t ihl, uint8_t version, uint8_t tos, uint8_t ttl)
{
	ip->ipV.v4.ihl = ihl;
	ip->ipV.v4.version = version;
	ip->ipV.v4.tos = tos;
	ip->ipV.v4.tot_len = htons(len);
	ip->ipV.v4.id = htons(id);
	ip->ipV.v4.frag_off = htons(frag_off);
	ip->ipV.v4.ttl = ttl;
	ip->ipV.v4.protocol = protocol;
	ip->ipV.v4.check = 0;

	ip->ipV.v4.saddr = src_ip->s_addr;
	ip->ipV.v4.daddr = dst_ip->s_addr;

	ip->ipV.v4.check = checksum((unsigned short *)ip, sizeof(ip_header));
}