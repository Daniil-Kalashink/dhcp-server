#ifndef DHCP_NET_H
#define DHCP_NET_H

#include "dhcp_common.h"

#define DHCP_NET_ERR -1
#define DHCP_NET_OK   0

#define IS_ERROR(func) func == DHCP_NET_ERR

int dhcp_net_create_sock(int * fd, int domain, int type, int proto, bool val_set_opt, int level, int optname);
int dhcp_net_udp_inet_sock(uint32_t addr, uint16_t port, struct sockaddr_in * serv);
int dhcp_net_set_opt(int fd, int level, int optname);
int dhcp_net_udp_local_sock(char * path, struct sockaddr_un * addr);
int dhcp_net_udp_unix_server(char * path, struct sockaddr_un * addr);
int dhcp_net_udp_unix_client(char * path, struct sockaddr_un * addr);
int dhcp_net_get_mac_address(const char * interface_name, uint8_t *inter_mac);
int dhcp_net_raw_inet_sock(int * fd, struct sockaddr_ll * dest_addr, char * interface, int8_t * target_mac);

#endif /* DHCP_NET_H */