cmake_minimum_required(VERSION 3.10)
project(dhcp-server VERSION 1.0.0 LANGUAGES C)

# Основной исполняемый файл
add_executable(dhcp-server 
    server/dhcp.c
)

target_include_directories(dhcp-server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/../include/server
)