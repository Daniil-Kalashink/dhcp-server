#include "include/wrap.h"

static bool mock_snprintf_enabled = true;
static bool mock_socket_enabled = true;
static bool mock_setsockopt_enabled = true;
static bool mock_bind_enabled = true;

void mock_snprintf_set_enabled(bool enabled)
{
	mock_snprintf_enabled = enabled;
}
void mock_socket_set_enabled(bool enabled)
{
	mock_socket_enabled = enabled;
}
void mock_setsockopt_set_enabled(bool enabled)
{
	mock_setsockopt_enabled = enabled;
}
void mock_bind_set_enabled(bool enabled)
{
	mock_bind_enabled = enabled;
}

int __wrap_snprintf(char *str, size_t size, const char *format, ...)
{
	if (!mock_snprintf_enabled)
	{
		va_list args;
		va_start(args, format);
		int result = vsnprintf(str, size, format, args);
		va_end(args);
		return result;
	}

	return (int)mock();
}

int __wrap_socket(int domain, int type, int protocol)
{
	if (!mock_socket_enabled)
	{
		return socket(domain, type, protocol);
	}

	return (int)mock();
}

int __wrap_setsockopt(int __fd, int __level, int __optname, const void *__optval, socklen_t __optlen)
{
	if (!mock_setsockopt_enabled)
	{
		return setsockopt(__fd, __level, __optname, __optval, __optlen);
	}

	return (int)mock();
}

int __wrap_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (!mock_bind_enabled)
	{
		return bind(sockfd, addr, addrlen);
	}

	return (int)mock();
}