#ifndef TEST_DHCP_NET_H
#define TEST_DHCP_NET_H

#include "include/wrap.h"
#include "../include/server/dhcp_net.h"
#include "../include/server/dhcp.h"

extern int dhcp_net_create_sock(int * fd, int domain, int type);
extern int dhcp_net_udp_inet_sock(uint32_t addr, uint16_t port, struct sockaddr_in * serv);
extern int dhcp_net_set_reuse_addr(int fd);
extern int dhcp_net_udp_local_sock(char * path, struct sockaddr_un * addr);
extern int dhcp_net_udp_unix_server(char * path, struct sockaddr_un * addr);
extern int dhcp_net_udp_unix_client(char * path, struct sockaddr_un * addr);

#endif /* TEST_DHCP_NET_H */