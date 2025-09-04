#ifndef DHCP_COMMON_H
#define DHCP_COMMON_H

// 1. Стандартные системные заголовки
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// 2. Системные сетевые заголовки (важен порядок!)
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <fcntl.h>
#include <errno.h>

// 3. Потоковые и системные заголовки
#include <pthread.h>
#include <syslog.h>

// 4. Кастомные типы и утилиты
#include "cdefs.h"

// 5. Логирование
#include "logger.h"



#endif /* DHCP_COMMON_H */