#ifndef DHCP_H
#define DHCP_H

#include "dhcp_common.h"

#include "netinet/eth.h"
#include "netinet/ipv4.h"
#include "netinet/udp.h"

#include "dhcp_net.h"
#include "logger.h"

#define DHCP_PORT_SERVER 67
#define DHCP_MTU_MAX     1500
#define DHCP_MTU_MIN     576
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

int server_for_receiving(int8_t * mac);
#endif /* DHCP_H */