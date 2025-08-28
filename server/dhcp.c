#include "include/server/dhcp.h"

void* server_for_receiving(void* arg) {

	int recv_fd;
	char buffer[DHCP_MTU_MAX];
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	memset(buffer, 0, sizeof(buffer));

	recv_fd = dhcp_net_udp_inet_sock(INADDR_ANY, DHCP_PORT_SERVER, &serv_addr);
	if (recv_fd < 0)
	{
		printf("[ERROR] dhcp_net_udp_local_sock\n");
		goto err;
	}

	while (1) {
		if (recvfrom(recv_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len) < 0) {
			perror("recvfrom failed");
			continue;
		}
	}

	err:
		pthread_exit(NULL);
}

int main()
{
	pthread_t thread_serv_resv;

	printf("[+] Main thread started. Creating DHCP server thread...\n");

	if (pthread_create(&thread_serv_resv, NULL, server_for_receiving, NULL) != 0) {
		perror("[-] Failed to create thread");
		return EXIT_FAILURE;
	}

	pthread_join(thread_serv_resv, NULL);
	
	printf("[-] Server thread exited. Shutting down.\n");
	return EXIT_SUCCESS;
}