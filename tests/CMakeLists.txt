cmake_minimum_required(VERSION 3.10)
project(test_dhcp VERSION 1.0.0 LANGUAGES C)

find_package(cmocka REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}/../include
	${CMAKE_SOURCE_DIR}
)

add_library(dhcp-server 
	../server/logger.c
	../server/dhcp_net.c
	../server/dhcp.c
)

target_include_directories(dhcp-server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

set(TEST_TARGETS 
)

foreach(test_target IN LISTS TEST_TARGETS)
	add_executable(${test_target}
		${test_target}.c
		wrap.c
	)

	target_link_libraries(${test_target} dhcp-server cmocka)

	target_link_options(${test_target} PRIVATE
		-Wl,--wrap=socket
		-Wl,--wrap=bind
		-Wl,--wrap=setsockopt
		-Wl,--wrap=snprintf
	)

	add_test(NAME ${test_target} COMMAND ${test_target})
endforeach()