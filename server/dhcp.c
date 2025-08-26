#include "include/server/dhcp.h"
// void* data_transfer(void* arg)
// {
// 	tid = (long)arg;
// 	pthread_exit(NULL);
// }

// int sock_dgram_init(socket_object_t * sock, int cnt_sock, int family)
// {
	
// 	memset(&socket_object_s, 0, sizeof(socket_object_t));
// 	for (int i = 0; i < cnt_sock; i++)
// 	{
// 		socket_object_t sock_elem = sock[i];
// 		sock_elem.fd = socket(family, SOCK_DGRAM, 0);
// 		if (sock_elem.fd < 0) {
// 			perror("sock_init - socket error");
// 			goto err;
// 		}

// 		struct sockaddr_in servaddr;
// 		memset(&servaddr, 0, sizeof(struct sockaddr_in));
// 		servaddr.sin_family = family;
// 		servaddr.sin_addr.s_addr = INADDR_ANY;
// 		servaddr.sin_port = htons(PORT_SERVER);
// 		sock_elem.serv = servaddr;
// 	}
// }

void* server_for_receiving(void* arg) {

	int fd;
	char buffer[1024];
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	fd = dhcp_net_udp_inet_sock(INADDR_ANY, PORT_SERVER, &serv_addr);
	if (fd < 0)
	{
		printf("[ERROR] dhcp_net_udp_local_sock\n");
		goto err_close;
	}

	while (1) {
		ssize_t recv_len = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
		if (recv_len < 0) {
			perror("recvfrom failed");
			continue;
		}
		printf("recvfrom len:%ld\n", recv_len);
	}

	err_close:
		close(fd);

	err:
		pthread_exit(NULL);
}

int main() {

    pthread_t thread_id;

    printf("[+] Main thread started. Creating DHCP server thread...\n");

    // Создаем поток, который сам создаст и привяжет сокет
    if (pthread_create(&thread_id, NULL, server_for_receiving, NULL) != 0) {
        perror("[-] Failed to create thread");
        return EXIT_FAILURE;
    }


    pthread_join(thread_id, NULL);

    printf("[-] Server thread exited. Shutting down.\n");
    return EXIT_SUCCESS;
}