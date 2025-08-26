#!/bin/bash

# Скрипт установки DHCP сервера
set -e

echo "=== Установка DHCP сервера ==="

# Проверяем что мы в директории с проектом
if [ ! -f "CMakeLists.txt" ]; then
    echo "Ошибка: Запустите скрипт из корневой директории проекта!"
    exit 1
fi

# Проверяем зависимости
echo "Проверка зависимостей..."
if ! command -v cmake &> /dev/null; then
    echo "Ошибка: cmake не установлен! Установите: sudo apt install cmake"
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "Ошибка: make не установлен! Установите: sudo apt install make"
    exit 1
fi

# Создаем директорию для сборки
echo "Создание директории для сборки..."
mkdir -p build
cd build

# Конфигурируем CMake
echo "Конфигурация CMake..."
cmake .. -DCMAKE_INSTALL_PREFIX=/usr

# Собираем проект
echo "Сборка проекта..."
make -j$(nproc)

# Устанавливаем
echo "Установка в систему..."
sudo make install

# Активируем systemd сервис
echo "Настройка systemd сервиса..."
sudo systemctl daemon-reload
sudo systemctl enable dhcp-server

echo ""
echo "=== Установка завершена успешно! ==="
echo ""
echo "Для запуска сервиса выполните:"
echo "  sudo systemctl start dhcp-server"
echo ""
echo "Для проверки статуса:"
echo "  sudo systemctl status dhcp-server"
echo ""
echo "Для запуска вручную:"
echo "  dhcp-server -d /path/to/directory"
echo ""
echo "Логи сервиса:"
echo "  journalctl -u dhcp-server -f"