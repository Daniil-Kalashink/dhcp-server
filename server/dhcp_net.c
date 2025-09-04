#include "../include/dhcp_net.h"

int dhcp_net_create_sock(int * fd, int domain, int type, int proto, bool val_set_opt, int level, int optname)
{
	*fd = socket(domain, type, proto);
	if (*fd < 0) {
		dhcp_log_error("socket: %s", strerror(errno));
		return DHCP_NET_ERR;
	}

	if (val_set_opt && (IS_ERROR(dhcp_net_set_opt(*fd, level, optname))))
	{
		dhcp_log_error("dhcp_net_udp_local_sock");
		close(*fd);
		return DHCP_NET_ERR;
	}

	return DHCP_NET_OK;
}

int dhcp_net_udp_inet_sock(uint32_t addr, uint16_t port, struct sockaddr_in * serv)
{
	int fd;
	memset(serv, 0, sizeof(struct sockaddr_in));

	if (IS_ERROR(dhcp_net_create_sock(&fd, AF_INET, SOCK_DGRAM, IPPROTO_UDP, true, SOL_SOCKET, SO_REUSEADDR)))
	{
		dhcp_log_error("dhcp_net_udp_local_sock");
		return DHCP_NET_ERR;
	}

	serv->sin_family = AF_INET;
	serv->sin_addr.s_addr = addr;
	serv->sin_port = htons(port);

	if (bind(fd, (struct sockaddr*)serv, sizeof(struct sockaddr_in)) < 0)
	{
		dhcp_log_error("bind: %s", strerror(errno));
		close(fd);
		return DHCP_NET_ERR;
	}

	return fd;
}

int dhcp_net_udp_local_sock(char * path, struct sockaddr_un * addr)
{
	int fd;
	memset(addr, 0, sizeof(struct sockaddr_un));

	if (IS_ERROR(dhcp_net_create_sock(&fd, AF_INET, SOCK_DGRAM, IPPROTO_UDP, true, SOL_SOCKET, SO_REUSEADDR)))
	{
		dhcp_log_error("dhcp_net_udp_local_sock");
		return DHCP_NET_ERR;
	}

	addr->sun_family = AF_LOCAL;
	strncpy(addr->sun_path, path, sizeof(addr->sun_path) - 1);

	return fd;
}

int dhcp_net_udp_unix_server(char * path, struct sockaddr_un * addr)
{
	int fd = dhcp_net_udp_local_sock(path, addr);
	if(fd < 0)
	{
		dhcp_log_error("dhcp_net_udp_local_sock");
		return DHCP_NET_ERR;
	}

	unlink(path);

	if (bind(fd, (struct sockaddr*)addr, sizeof(struct sockaddr_un)) == -1)
	{
		dhcp_log_error("bind");
		close(fd);
		return DHCP_NET_ERR;
	}

	return fd;
}

int dhcp_net_udp_unix_client(char * path, struct sockaddr_un * addr)
{
	int fd = dhcp_net_udp_local_sock(path, addr);
	if(fd < 0)
	{
		dhcp_log_error("dhcp_net_udp_local_sock");
		return DHCP_NET_ERR;
	}

	return fd;
}

int dhcp_net_set_opt(int fd, int level, int optname)
{
	int optval = 1;

	if (setsockopt(fd, level, optname, &optval, sizeof(optval)) == -1) {
		dhcp_log_error("setsockopt SO_REUSEADDR: %s", strerror(errno));
		return DHCP_NET_ERR;
	}

	return DHCP_NET_OK;
}