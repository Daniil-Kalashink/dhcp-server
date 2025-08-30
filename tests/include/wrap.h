#ifndef WRAP_H
#define WRAP_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <setjmp.h>
#include <cmocka.h>

void mock_snprintf_set_enabled(bool enabled);
void mock_socket_set_enabled(bool enabled);
void mock_setsockopt_set_enabled(bool enabled);
void mock_bind_set_enabled(bool enabled);

int __wrap_snprintf(char *str, size_t size, const char *format, ...);
int __wrap_socket(int domain, int type, int protocol);
int __wrap_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int __wrap_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif /* WRAP_H */