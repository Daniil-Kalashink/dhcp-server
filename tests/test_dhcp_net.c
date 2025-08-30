#include "include/test_dhcp_net.h"

void test_dhcp_net_create_sock(void **state)
{
	int result;
	int fd;
	mock_snprintf_set_enabled(false);

	print_message("\n+================1)TEST=====================+\n");
	will_return(__wrap_socket, -1);
	result = dhcp_net_create_sock(&fd, AF_INET, SOCK_DGRAM);
	assert_int_equal(result, -1);
	print_message("\n+===========================================+\n");

	print_message("\n+================2)TEST=====================+\n");
	will_return(__wrap_socket, 0);
	will_return(__wrap_setsockopt, 0);
	result = dhcp_net_create_sock(&fd, AF_INET, SOCK_DGRAM);
	assert_int_equal(result, 0);
	print_message("\n+===========================================+\n");

	mock_snprintf_set_enabled(true);
}

void test_dhcp_net_udp_inet_sock(void **state)
{
	int result;
	int fd;
	struct sockaddr_in serv_addr;

	mock_snprintf_set_enabled(false);

	print_message("\n+================1)TEST=====================+\n");
	will_return(__wrap_bind, -1);
	will_return(__wrap_setsockopt, 0);
	will_return(__wrap_socket, 10);
	result = dhcp_net_udp_inet_sock(INADDR_ANY, DHCP_PORT_SERVER, &serv_addr);
	assert_int_equal(result, -1);
	print_message("\n+===========================================+\n");

	print_message("\n+================2)TEST=====================+\n");
	will_return(__wrap_bind, 4);
	will_return(__wrap_socket, 10);
	will_return(__wrap_setsockopt, 0);
	result = dhcp_net_udp_inet_sock(INADDR_ANY, DHCP_PORT_SERVER, &serv_addr);
	assert_int_equal(result, 10);
	print_message("\n+===========================================+\n");

	mock_snprintf_set_enabled(true);
}

void test_dhcp_net_set_reuse_addr(void **state)
{
	int result;
	int fd;
	mock_snprintf_set_enabled(false);

	print_message("\n+================1)TEST=====================+\n");
	will_return(__wrap_setsockopt, -1);
	result = dhcp_net_set_reuse_addr(fd);
	assert_int_equal(result, -1);
	print_message("\n+===========================================+\n");

	print_message("\n+================2)TEST=====================+\n");
	will_return(__wrap_setsockopt, 0);
	result = dhcp_net_set_reuse_addr(fd);
	assert_int_equal(result, 0);
	print_message("\n+===========================================+\n");

	mock_snprintf_set_enabled(true);
}

void test_dhcp_net_udp_local_sock(void **state)
{
	int result;
	struct sockaddr_un addr;

	mock_snprintf_set_enabled(false);

	will_return(__wrap_setsockopt, 0);

	print_message("\n+================1)TEST=====================+\n");
	will_return(__wrap_socket, -1);
	result = dhcp_net_udp_local_sock("/puth", &addr);
	assert_int_equal(result, -1);
	print_message("\n+===========================================+\n");

	print_message("\n+================2)TEST=====================+\n");
	will_return(__wrap_socket, 10);
	result = dhcp_net_udp_local_sock("/puth", &addr);
	assert_int_equal(result, 10);
	print_message("\n+===========================================+\n");

	mock_snprintf_set_enabled(true);
}

void test_dhcp_net_udp_unix_server(void **state)
{
		int result;
	struct sockaddr_un addr;

	mock_snprintf_set_enabled(false);


	print_message("\n+================1)TEST=====================+\n");
	will_return(__wrap_bind, -1);
	will_return(__wrap_socket, 10);
	will_return(__wrap_setsockopt, 0);
	result = dhcp_net_udp_unix_server("/puth", &addr);
	assert_int_equal(result, -1);
	print_message("\n+===========================================+\n");

	print_message("\n+================2)TEST=====================+\n");
	will_return(__wrap_bind, 0);
	will_return(__wrap_socket, 10);
	will_return(__wrap_setsockopt, 0);
	result = dhcp_net_udp_unix_server("/puth", &addr);
	assert_int_equal(result, 10);
	print_message("\n+===========================================+\n");

	mock_snprintf_set_enabled(true);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_dhcp_net_create_sock),
		cmocka_unit_test(test_dhcp_net_udp_inet_sock),
		cmocka_unit_test(test_dhcp_net_set_reuse_addr),
		cmocka_unit_test(test_dhcp_net_udp_local_sock),
		cmocka_unit_test(test_dhcp_net_udp_unix_server),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}