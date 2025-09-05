#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H

#include "include/wrap.h"
#include "../include/logger.h"

extern int dhcp_log_emerg(const char *format, ...);
extern int dhcp_log_error(const char *format, ...);
extern int dhcp_log_warn(const char *format, ...);
extern int dhcp_log_info(const char *format, ...);
extern int dhcp_log_debug(const char *format, ...);

#endif /* TEST_LOGGER_H */