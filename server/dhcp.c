#include "../include/dhcp.h"

int capture_dhcp_traffic(char *buffer, dhcp_parse_packet *packet)
{
	char *ptr_buf = buffer;
	size_t remaining_len = DHCP_MAX_SIZE_SOCK;
	/* Parse Ethernet Frame */
	memcpy(&packet->eth, ptr_buf, sizeof(eth_header));

	ptr_buf += sizeof(eth_header);
	remaining_len -= sizeof(eth_header);

	/* Parse IP Frame */
	switch (ntohs(packet->eth.ether_type))
	{
		case ETHERTYPE_IP: // IPV4
		{
			memcpy(&packet->ip.ipV, ptr_buf, sizeof(ip_header4));
		
			ip_header4 *ipv4 = &packet->ip.ipV.v4; 
			
			packet->ip.version = IP_VERSION_4;

			if ((ntohl(ipv4->daddr) != IP_BROADCAST) || (ipv4->protocol != IPPROTO_UDP))
				return 1;

			ptr_buf += sizeof(ip_header4);
			remaining_len -= sizeof(ip_header4);
			break;
		}
	
	default:
		return 1;
		break;
	}
	

	/* Parse UDP Frame */
	memcpy(&packet->udp, ptr_buf, sizeof(udp_header));

	if (ntohs(packet->udp.dest) != DHCP_PORT_SERVER)
		return 1;

	ptr_buf += sizeof(udp_header);
	remaining_len -= sizeof(udp_header);

	/* Parse DHCP Frame*/
	memcpy(&packet->dhcp, ptr_buf, remaining_len);

	return 0;
}

int dhcp_preprocessing(dhcp_parse_packet * packet, int8_t * mac)
{
	dhcp_packet * dhcp = &packet->dhcp;

	const char *ip_str = DHCP_IP;
	struct in_addr src_addr;
		
	if (inet_aton(ip_str, &src_addr) == 0) {
		dhcp_log_error("IP address transformation error\n");
		return -1;
	}

	if (dhcp->giaddr.s_addr != 0)
	{
		dhcp_log_error("giaddr");
		return -2; // There will be a code with here later BOOTP
	}

	if (dhcp->ciaddr.s_addr != 0)
	{
		dhcp_log_error("ciaddr");
		return 0;
	}

	if (BROADCAST_FLAG(dhcp->flags) != 0)
	{
		dhcp_log_error("BROADCAST_FLAG");
		return 0;
	}
	else
	{
		int8_t src_mac[6];
		memcpy(src_mac, dhcp->chaddr, 6);
		
		dhcp_init_eth_header(&packet->eth, mac, src_mac);
		switch (packet->ip.version)
		{
			case IP_VERSION_4:
				dhcp_init_ipv4_header(&packet->ip, &(src_addr), &(dhcp->yiaddr), 0);
				break;
			default:
				break;
		}
		dhcp_init_udp_header(&packet->udp, 0 ,DHCP_PORT_SERVER, DHCP_PORT_CLIENT);

		return 0;
	}

	return -1;
}
int server_for_receiving(int8_t * mac)
{
	int fd;
	char buf[DHCP_MAX_SIZE_SOCK];
	struct sockaddr_ll dest_addr;
	socklen_t dest_len = sizeof(dest_addr);

	dhcp_parse_packet packet;

	if (dhcp_net_raw_inet_sock(&fd, &dest_addr, "eth0", mac) < 0)
	{
		dhcp_log_error("dhcp_net_udp_local_sock failed");
		return -1;
	}

	while (1) {
		memset(&packet, 0, sizeof(dhcp_parse_packet));
		memset(buf, 0, sizeof(buf));
		if (recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&dest_addr, &dest_len) < 0) {
			dhcp_log_error("recvfrom failed");
			continue;
		}

		if(capture_dhcp_traffic(buf, &packet))
			continue;

		if(dhcp_preprocessing(&packet, mac))
			continue;
}

int main()
{
	int fd;
	pthread_t thread_serv_resv;
	int8_t mac[6];

	srand(time(NULL));

	fd = open("/dev/null", O_RDWR);
	if (fd == 0)
		fd = open("/dev/null", O_RDWR);
	if (fd == 1)
		fd = open("/dev/null", O_RDWR);
	if (fd == 2)
		log_perror = 0; /* No sense logging to /dev/null. */
	else if (fd != -1)
		close(fd);

	setup_syslog(LOG_LEVEL_DEBUG, "dhcp-server");
	
	if (dhcp_net_get_mac_address("eth0", mac))
	{
		dhcp_log_emerg("Couldn't find a MAC address for this interface.");
		return EXIT_SUCCESS;
	}

	dhcp_log_info("[+] Main thread started. Creating DHCP server thread...\n");

	if (server_for_receiving(mac) != 0) {
		dhcp_log_emerg("Failed to create thread");
		return EXIT_FAILURE;
	}

	closelog();

	dhcp_log_info("[-] Server thread exited. Shutting down.\n");
	return EXIT_SUCCESS;
}