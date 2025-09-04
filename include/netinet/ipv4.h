#ifndef IPV4_H
#define IPV4_H

#include "dhcp_common.h"

typedef struct
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	uint8_t  ihl:4;         // Internet Header Length
	uint8_t  version:4;     // Version
#else
	uint8_t  version:4;     // Version
	uint8_t  ihl:4;         // Internet Header Length
#endif
	uint8_t  tos;           // Type of Service
	uint16_t tot_len;       // Total Length
	uint16_t id;            // Identification
	uint16_t frag_off;      // Fragment Offset
	uint8_t  ttl;           // Time to Live
	uint8_t  protocol;      // Protocol
	uint16_t check;         // Header Checksum
	uint32_t saddr;         // Source Address
	uint32_t daddr;         // Destination Address
} __attribute__((packed)) ip_header4;

/*Dan DHCP does not support IPV6, but for the future*/
typedef struct
{
	char temp;
} __attribute__((packed)) ip_header6;

typedef struct {
	union{
		ip_header4 v4;
		ip_header6 v6;
	} ipV;
	enum {
		IP_NONE,
		IP_VERSION_4,
		IP_VERSION_6
	} version;
} ip_header;

unsigned short checksum(void *b, int len);
void init_ipv4_header(ip_header *ip, uint16_t id, uint16_t frag_off, uint8_t protocol, uint16_t len,
         struct in_addr *src_ip, struct in_addr *dst_ip, uint8_t ihl, uint8_t version, uint8_t tos, uint8_t ttl);
#endif /*IPV4_H*/