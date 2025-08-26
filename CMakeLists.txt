cmake_minimum_required(VERSION 3.10)
project(dhcp-server VERSION 1.0.0 LANGUAGES C)

# Создаем исполняемый файл
add_executable(dhcp-server 
server/dhcp_net.c
server/dhcp.c
)

# Добавляем пути для include файлов конкретно для этого target
target_include_directories(dhcp-server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Устанавливаем права доступа
set_target_properties(dhcp-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Установка в систему
install(TARGETS dhcp-server
    DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
                GROUP_READ GROUP_EXECUTE 
                WORLD_READ WORLD_EXECUTE
)

# Создаем systemd service файл
configure_file(
    systemd/dhcp-server.service.in
    ${CMAKE_BINARY_DIR}/dhcp-server.service
    @ONLY
)

# Устанавливаем systemd service
install(FILES ${CMAKE_BINARY_DIR}/dhcp-server.service
    DESTINATION /etc/systemd/system
)