#!/bin/sh
# Builds the firmware modules for the host PC against mocked AVR headers and runs the unit tests.
# Usage (from the repository root): sh test/host/run_tests.sh      needs a host C compiler (gcc or clang)
set -e
cd "$(dirname "$0")/../.."
mkdir -p test/host/build
${CC:-gcc} -std=c11 -Wall -Wextra -Werror -DF_CPU=16000000UL \
	-I test/host/mock -I lib/MCAL -I lib/HAL -I lib/Service -I src/APP \
	test/host/test_main.c test/host/mock/mock_regs.c \
	lib/MCAL/USART/USART.c lib/HAL/HCSR04/HCSR04.c lib/HAL/BUZZER/BUZZER.c lib/HAL/LED/LED.c \
	lib/Service/SYSTICK/SYSTICK.c src/APP/COLLISION/COLLISION.c \
	-o test/host/build/run_tests
./test/host/build/run_tests
