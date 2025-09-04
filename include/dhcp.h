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

void *server_for_receiving(void* arg);

#endif /* DHCP_H */