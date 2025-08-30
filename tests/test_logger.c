#include "include/test_logger.h"

typedef int (*dhcp_log_func) (const char *format, ...);


static void test_dhcp_log(dhcp_log_func func)
{
	int result;
	print_message("\n+================1)TEST=====================+\n");
	will_return(__wrap_snprintf, -1);
	result = func("Temp message");
	assert_int_equal(result, 0);
	print_message("\n+===========================================+\n");

	print_message("\n+================2)TEST=====================+\n");
	will_return(__wrap_snprintf, MAX_LOG_BUF-1);
	result = func("Temp message");
	assert_int_equal(result, 0);
	print_message("\n+===========================================+\n");

	print_message("\n+================3)TEST=====================+\n");
	will_return(__wrap_snprintf, MAX_LOG_BUF+1);
	result = func("Temp message");
	assert_int_equal(result, 0);
	print_message("\n+===========================================+\n");
}

void test_dhcp_log_emerg(void ** stage)
{
	test_dhcp_log(dhcp_log_emerg);
}

void test_dhcp_log_error(void ** stage)
{
	test_dhcp_log(dhcp_log_error);
}

void test_dhcp_log_warn(void ** stage)
{
	test_dhcp_log(dhcp_log_warn);
}

void test_dhcp_log_info(void ** stage)
{
	test_dhcp_log(dhcp_log_info);
}

void test_dhcp_log_debug(void ** stage)
{
	test_dhcp_log(dhcp_log_debug);
}


int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_dhcp_log_emerg),
		cmocka_unit_test(test_dhcp_log_error),
		cmocka_unit_test(test_dhcp_log_warn),
		cmocka_unit_test(test_dhcp_log_info),
		cmocka_unit_test(test_dhcp_log_debug),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}