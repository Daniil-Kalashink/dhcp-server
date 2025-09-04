#include "../include/dhcp.h"
	{
		dhcp_log_error("[ERROR] dhcp_net_udp_local_sock\n");
		goto err;
	}

	while (1) {
		if (recvfrom(recv_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len) < 0) {
			dhcp_log_error("recvfrom failed");
			continue;
		}
	}

	err:
		pthread_exit(NULL);
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

	if (pthread_create(&thread_serv_resv, NULL, server_for_receiving, NULL) != 0) {
		perror("[-] Failed to create thread");
		return EXIT_FAILURE;
	}

	pthread_join(thread_serv_resv, NULL);
	closelog();

	dhcp_log_info("[-] Server thread exited. Shutting down.\n");
	return EXIT_SUCCESS;
}