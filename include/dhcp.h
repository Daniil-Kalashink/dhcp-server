#ifndef DHCP_H
#define DHCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>
#include "dhcp_net.h"
#include "logger.h"

#define DHCP_PORT_SERVER 67
#define DHCP_MTU_MAX     1500
#define DHCP_MTU_MIN     576

void *server_for_receiving(void* arg);

#endif /* DHCP_H */