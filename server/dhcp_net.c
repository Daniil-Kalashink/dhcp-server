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

int dhcp_net_get_mac_address(const char *interface_name, uint8_t *inter_mac)
{
	int fd;
	struct ifreq ifr;

	// Проверка входных параметров
	if (interface_name == NULL || inter_mac == NULL) {
		dhcp_log_error("Invalid parameters");
		return DHCP_NET_ERR;
	}

	// Создаем сокет
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		dhcp_log_error("socket creation failed");
		return DHCP_NET_ERR;
	}

	// Инициализируем структуру ifreq
	memset(&ifr, 0, sizeof(ifr));

	// Устанавливаем имя интерфейса
	strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0'; // Гарантируем null-termination

	// Получаем MAC-адрес
	if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
		dhcp_log_error("ioctl SIOCGIFHWADDR failed");
		close(fd);
		return DHCP_NET_ERR;
	}

	// Копируем MAC-адрес в выходной буфер
	memcpy(inter_mac, ifr.ifr_hwaddr.sa_data, 6);

	close(fd);
	return DHCP_NET_OK;
}

int dhcp_net_raw_inet_sock(int * fd, struct sockaddr_ll * dest_addr, char * interface, int8_t * target_mac)
{	
	if (dest_addr == NULL)
		return DHCP_NET_ERR;

	if (IS_ERROR(dhcp_net_create_sock(fd, AF_PACKET, SOCK_RAW, htons(ETH_P_ALL), false, 0, 0)))
	{
		dhcp_log_error("dhcp_net_udp_local_sock");
		return DHCP_NET_ERR;
	}
	struct ifreq ifr;
	strncpy(ifr.ifr_name, interface, IFNAMSIZ);
	if (ioctl(*fd, SIOCGIFINDEX, &ifr) < 0) {
		perror("ioctl SIOCGIFINDEX");
		close(*fd);
		return DHCP_NET_ERR;
	}

	memset(dest_addr, 0, sizeof(struct sockaddr_ll));
	dest_addr->sll_family = AF_PACKET;
	dest_addr->sll_protocol = htons(ETH_P_ALL);
	dest_addr->sll_ifindex = ifr.ifr_ifindex;
	dest_addr->sll_halen = ETH_ALEN;
	memcpy(dest_addr->sll_addr, target_mac, ETH_ALEN);

	if (bind(*fd, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) < 0) {
		dhcp_log_error("Error binding to the interface");
		close(*fd);
		return DHCP_NET_ERR;
	}

	return DHCP_NET_OK;
}