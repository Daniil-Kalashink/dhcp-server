#!/bin/bash

# Скрипт удаления DHCP сервера
set -e

echo "=== Удаление DHCP сервера ==="

# Останавливаем и отключаем сервис
echo "Остановка сервиса..."
sudo systemctl stop dhcp-server 2>/dev/null || true
sudo systemctl disable dhcp-server 2>/dev/null || true

# Если есть директория сборки - удаляем через make uninstall
if [ -d "build" ]; then
    echo "Удаление файлов..."
    cd build
    sudo make uninstall 2>/dev/null || true
    cd ..
    rm -rf build
fi

# Удаляем вручную на случай если make uninstall не сработал
echo "Очистка оставшихся файлов..."
sudo rm -f /usr/bin/dhcp-server 2>/dev/null || true
sudo rm -f /etc/systemd/system/dhcp-server.service 2>/dev/null || true

# Перезагружаем демон systemd
sudo systemctl daemon-reload

echo ""
echo "=== Удаление завершено успешно! ==="