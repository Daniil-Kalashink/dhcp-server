#ifndef DHCP_NET_H
#define DHCP_NET_H

#include "dhcp_common.h"

#define DHCP_NET_ERR -1
#define DHCP_NET_OK   0

#define IS_ERROR(func) func == DHCP_NET_ERR

int dhcp_net_create_sock(int * fd, int domain, int type);
int dhcp_net_udp_inet_sock(uint32_t addr, uint16_t port, struct sockaddr_in * serv);
int dhcp_net_set_reuse_addr(int fd);
int dhcp_net_udp_local_sock(char * path, struct sockaddr_un * addr);
int dhcp_net_udp_unix_server(char * path, struct sockaddr_un * addr);
int dhcp_net_udp_unix_client(char * path, struct sockaddr_un * addr);

#endif /* DHCP_NET_H */