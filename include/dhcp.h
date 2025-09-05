/* dhcp.h
 * Modified DHCP packet structure for educational project
 */

/*
 * Copyright (C) 2004-2022 Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1995-2003 by Internet Software Consortium
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *   Internet Systems Consortium, Inc.
 *   PO Box 360
 *   Newmarket, NH 03857 USA
 *   <info@isc.org>
 *   https://www.isc.org/
 */

/* Modifications for educational project:
 * - Added pragma package for tight packing of dhcp_packet structure.
 *
 * Modifications Copyright (C) 2025 Daniil Kalashnik
 * This modified work is also subject to the terms of the MPL-2.0 license.
 */

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