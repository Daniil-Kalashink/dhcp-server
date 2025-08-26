cmake_minimum_required(VERSION 3.10)
project(test_dhcp VERSION 1.0.0 LANGUAGES C)

# Ищем CMocka
find_package(cmocka REQUIRED)

# Добавляем путь к заголовкам основного проекта
include_directories(
	${CMAKE_SOURCE_DIR}/../include  # путь к include из директории tests
	${CMAKE_SOURCE_DIR}			# текущая директория тестов
)

# Создаем библиотеку с исходным кодом
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
	test_dhcp_net
	# test_logger  # изменил название, чтобы избежать конфликта
)

# Тестовый исполняемый файл
foreach(test_target IN LISTS TEST_TARGETS)
	add_executable(${test_target}
		${test_target}.c
		wrap.c
	)

	target_link_libraries(${test_target} dhcp-server cmocka)

	# Флаги для подмены функций
	target_link_options(${test_target} PRIVATE
		-Wl,--wrap=socket
		-Wl,--wrap=bind
		-Wl,--wrap=setsockopt
		-Wl,--wrap=snprintf
	)

	# Добавляем тест в CTest
	add_test(NAME ${test_target} COMMAND ${test_target})
endforeach()