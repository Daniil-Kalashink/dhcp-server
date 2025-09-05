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
}

int main()
{
	int fd;
	pthread_t thread_serv_resv;

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