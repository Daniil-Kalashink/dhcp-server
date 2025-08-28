#include "include/server/logger.h"

#ifdef DEBUG
	int log_perror = 1;
#else
	int log_perror = -1;
#endif

static log_level_t current_log_level = LOG_LEVEL_INFO;

static int dhcp_format(char * buf, char * level, char * format_buf)
{
	int count;
	count=snprintf(format_buf, MAX_LOG_BUF, DHCP_FORMAT, level, buf);
	if (count < 0 || count >= MAX_LOG_BUF)
		return -1;
	
	return 0;
}

void setup_syslog(log_level_t level, char * name)
{
	int log_options = LOG_NDELAY;

	current_log_level = level;

	switch (level)
	{
		case LOG_LEVEL_EMERG:
			setlogmask(LOG_UPTO(LOG_EMERG));
			break;
		case LOG_LEVEL_ERROR:
			setlogmask(LOG_UPTO(LOG_ERR));
			break;
		case LOG_LEVEL_WARNING:
			setlogmask(LOG_UPTO(LOG_WARNING));
			break;
		case LOG_LEVEL_INFO:
			setlogmask(LOG_UPTO(LOG_INFO));
			break;
		case LOG_LEVEL_DEBUG:
			setlogmask(LOG_UPTO(LOG_DEBUG));
			break;
		default:
			break;
	}

	openlog(name, log_options, LOG_USER);
}

int dhcp_log_emerg(const char *format, ...)
{
	char buf[MAX_LOG_BUF];
	char format_buf[MAX_LOG_BUF];
	buf[0] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if (dhcp_format(buf, "EMERG", format_buf))
	{
		IGNORE_RET(write(STDERR_FILENO, "Function dhcp_format in dhcp_log_emerg failed with error\n", 57));
		return 0;
	}

#ifndef DEBUG
	syslog (LOG_EMERG, "%s", format_buf);
#endif

	if (log_perror && current_log_level>=LOG_LEVEL_EMERG) {
		IGNORE_RET(write(STDERR_FILENO, format_buf, strlen(format_buf)));
		IGNORE_RET(write (STDERR_FILENO, "\n", 1));
	}

	return 0;
}

int dhcp_log_crit(const char *format, ...)
{
	char buf[MAX_LOG_BUF];
	char format_buf[MAX_LOG_BUF];
	buf[0] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if (dhcp_format(buf, "CRIT", format_buf))
	{
		IGNORE_RET(write(STDERR_FILENO, "Function dhcp_format in dhcp_log_crit failed with error\n", 56));
		return 0;
	}

#ifndef DEBUG
	syslog (LOG_CRIT, "%s", format_buf);
#endif

	if (log_perror && current_log_level>=LOG_LEVEL_CRIT) {
		IGNORE_RET(write(STDERR_FILENO, format_buf, strlen(format_buf)));
		IGNORE_RET(write (STDERR_FILENO, "\n", 1));
	}

	return 0;
}

int dhcp_log_warn(const char *format, ...)
{
	char buf[MAX_LOG_BUF];
	char format_buf[MAX_LOG_BUF];
	buf[0] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if (dhcp_format(buf, "WARNING", format_buf))
	{
		IGNORE_RET(write(STDERR_FILENO, "Function dhcp_format in dhcp_log_warn failed with error\n", 56));
		return 0;
	}

#ifndef DEBUG
	syslog (LOG_WARNING, "%s", format_buf);
#endif

	if (log_perror && current_log_level>=LOG_LEVEL_WARNING) {
		IGNORE_RET(write(STDERR_FILENO, format_buf, strlen(format_buf)));
		IGNORE_RET(write (STDERR_FILENO, "\n", 1));
	}

	return 0;
}

int dhcp_log_error(const char *format, ...)
{
	char buf[MAX_LOG_BUF];
	char format_buf[MAX_LOG_BUF];
	buf[0] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if (dhcp_format(buf, "ERROR", format_buf))
	{
		IGNORE_RET(write(STDERR_FILENO, "Function dhcp_format in dhcp_log_error failed with error\n", 57));
		return 0;
	}

#ifndef DEBUG
	syslog (LOG_INFO, "%s", format_buf);
#endif

	if (log_perror && current_log_level>=LOG_LEVEL_ERROR) {
		IGNORE_RET(write(STDERR_FILENO, format_buf, strlen(format_buf)));
		IGNORE_RET(write (STDERR_FILENO, "\n", 1));
	}

	return 0;
}

int dhcp_log_info(const char *format, ...)
{
	char buf[MAX_LOG_BUF];
	char format_buf[MAX_LOG_BUF];
	buf[0] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if (dhcp_format(buf, "INFO", format_buf))
	{
		IGNORE_RET(write(STDERR_FILENO, "Function dhcp_format in dhcp_log_info failed with error\n", 56));
		return 0;
	}

#ifndef DEBUG
	syslog (LOG_INFO, "%s", format_buf);
#endif

	if (log_perror && current_log_level>=LOG_LEVEL_INFO) {
		IGNORE_RET(write(STDERR_FILENO, format_buf, strlen(format_buf)));
		IGNORE_RET(write (STDERR_FILENO, "\n", 1));
	}

	return 0;
}

int dhcp_log_debug(const char *format, ...)
{
	char buf[MAX_LOG_BUF];
	char format_buf[MAX_LOG_BUF];
	buf[0] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if (dhcp_format(buf, "DEBUG", format_buf))
	{
		IGNORE_RET(write(STDERR_FILENO, "Function dhcp_format in dhcp_log_debug failed with error\n", 57));
		return 0;
	}

#ifndef DEBUG
	syslog (LOG_DEBUG, "%s", format_buf);
#endif

	if (log_perror && current_log_level>=LOG_LEVEL_DEBUG) {
		IGNORE_RET(write(STDERR_FILENO, format_buf, strlen(format_buf)));
		IGNORE_RET(write (STDERR_FILENO, "\n", 1));
	}

	return 0;
}