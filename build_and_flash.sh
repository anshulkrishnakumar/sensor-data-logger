#!/bin/bash

set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROGRAMMER_CLI="$HOME/.local/share/stm32cube/bundles/programmer/2.23.0/bin/STM32_Programmer_CLI"
FIRMWARE="$PROJECT_DIR/build/Debug/sensor-data-logger.elf"

cd "$PROJECT_DIR"

echo "=== Configuring project ==="
cmake --preset Debug

echo
echo "=== Building firmware ==="
cmake --build --preset Debug

echo
echo "=== Flashing firmware ==="
"$PROGRAMMER_CLI" \
    -c port=SWD \
    -w "$FIRMWARE" \
    -v \
    -rst

echo
echo "=== Build and flash complete ==="