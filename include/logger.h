#ifndef LOGGER_H
#define LOGGER_H

#include "dhcp_common.h"

extern int log_perror;

#define MAX_LOG_BUF 1024

#define DHCP_FORMAT "[%s]:%s"

typedef enum {
	LOG_LEVEL_EMERG = 0,
	LOG_LEVEL_ALERT,
	LOG_LEVEL_CRIT,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_NOTICE,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
} log_level_t;

void setup_syslog(log_level_t level, char * name);
int dhcp_log_emerg(const char *format, ...);
int dhcp_log_error(const char *format, ...);
int dhcp_log_warn(const char *format, ...);
int dhcp_log_info(const char *format, ...);
int dhcp_log_debug(const char *format, ...);

#endif /* LOGGER_H */