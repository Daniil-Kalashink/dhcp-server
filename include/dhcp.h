#ifndef DHCP_H
#define DHCP_H

#include "dhcp_common.h"

#include "netinet/eth.h"
#include "netinet/ipv4.h"
#include "netinet/udp.h"

#include "dhcp_net.h"

#define BROADCAST_FLAG(flag) flag & 0x8000

#define DHCP_PORT_SERVER 67
#define DHCP_PORT_CLIENT 68
#define DHCP_MTU_MAX     1500
#define DHCP_MTU_MIN     576
#define DHCP_TTL         128
#define DHCP_IP          "172.20.0.4"

/* IP header + UDP header */
#define DHCP_UDP_OVERHEAD     (20 + 8)
#define DHCP_ETH_FRAME        14
#define DHCP_FCS              4
#define DHCP_SNAME_LEN        64
#define DHCP_FILE_LEN         128
#define DHCP_FIXED_NON_UDP    236
#define DHCP_CHADDR_LEN       16

#define DHCP_FIXED_LEN        (DHCP_FIXED_NON_UDP + DHCP_UDP_OVERHEAD)

#define DHCP_MAX_OPTION_LEN   (DHCP_MTU_MAX - DHCP_FIXED_LEN)
#define DHCP_MIN_OPTION_LEN   (DHCP_MTU_MIN - DHCP_FIXED_LEN)

/* MAX MTU+ Ethernet frame + FSC */
#define DHCP_MAX_SIZE_SOCK (DHCP_ETH_FRAME+DHCP_MTU_MAX)

#define IP_BROADCAST 0xffffffff

#define dhcp_init_eth_header(eth, src_mac, dst_mac)\
	init_eth_header(eth, src_mac, dst_mac, ETHERTYPE_IP)\

#define dhcp_init_ipv4_header(ip, src_ip, dst_ip, len)\
	init_ipv4_header(ip, rand()%65536, 0, IPPROTO_UDP, len, src_ip, dst_ip, 5, 4, 0, DHCP_TTL)\

#define dhcp_init_udp_header(udp, len, src_port, dst_port)\
	init_udp_header(udp, len, 0, src_port, dst_port)\
#pragma pack(push, 1)
typedef struct {
	uint8_t op;                           // Operation code
	uint8_t htype;                        // Hardware type
	uint8_t hlen;                         // Hardware length
	uint8_t hops;                         // Hops
	uint32_t xid;                         // Transaction ID
	uint16_t secs;                        // Seconds
	uint16_t flags;                       // Flags
	struct in_addr ciaddr;                // Client IP address
	struct in_addr yiaddr;                // Your IP address
	struct in_addr siaddr;                // Next server IP address
	struct in_addr giaddr;                // Relay agent IP address
	unsigned char chaddr[DHCP_CHADDR_LEN];       // Client hardware address
	char sname[DHCP_SNAME_LEN];           // Server name
	char file[DHCP_FILE_LEN];             // Boot filename
	uint8_t options[DHCP_MAX_OPTION_LEN]; // DHCP options
} dhcp_packet;
#pragma pack(pop)

typedef struct
{
	eth_header  eth;
	ip_header   ip;
	udp_header  udp;
	dhcp_packet dhcp;
} dhcp_parse_packet;

int server_for_receiving(int8_t * mac);
int dhcp_preprocessing(dhcp_parse_packet * packet, int8_t * mac);
int capture_dhcp_traffic(char *buffer, dhcp_parse_packet *packet);
#endif /* DHCP_H */