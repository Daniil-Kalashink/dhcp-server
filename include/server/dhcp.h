#ifndef DHCP_H
#define DHCP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "include/server/dhcp_net.h"

#define PORT_SERVER 67

void *server_for_receiving(void* arg);

#endif /* DHCP_H */