# Makefile — прошивка-исполнитель (Вариант 1) + хостовые тесты/линт.
# Всё гоняется в docker-образе mower-fwbuild (см. Dockerfile.build).

INC       := include
PORT_SRC  := src/mx_crc8.c src/mx_proto.c src/mx_mixer.c src/mx_safety.c
HOST_SRC  := $(PORT_SRC) src/mx_executor.c
TEST_SRC  := tests/run_tests.c $(HOST_SRC)

WARN      := -Wall -Wextra -Werror -Wshadow -Wconversion -Wsign-conversion \
             -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
             -Wundef -Wdouble-promotion
CSTD      := -std=c11

# ---- ARM (cross) ----
ARM_CC    := arm-none-eabi-gcc
ARM_OBJ   := arm-none-eabi-objcopy
ARM_SIZE  := arm-none-eabi-size
MCU       := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
ARM_SRC   := $(PORT_SRC) src/mx_vendor.c src/mx_executor.c src/mx_hook.c
ARM_FLAGS := $(MCU) $(CSTD) $(WARN) -Os -ffreestanding -fno-common \
             -ffunction-sections -fdata-sections -I$(INC)
LDSCRIPT  := ld/executor.ld
BUILD     := build

.PHONY: all verify test asan lint analyze fw clean

# Полная проверка софта (то, что гоняем до перехода к железу)
verify: test asan lint analyze fw
	@echo "=== ВСЕ ПРОВЕРКИ ПРОЙДЕНЫ ==="

all: test lint fw

# ---- Хостовые юнит-тесты ----
test:
	gcc $(CSTD) $(WARN) -O2 -I$(INC) $(TEST_SRC) -o $(BUILD)/run_tests
	$(BUILD)/run_tests

# ---- Санитайзеры (ASan+UBSan) ----
asan:
	gcc $(CSTD) $(WARN) -O1 -g -fsanitize=address,undefined \
	    -fno-sanitize-recover=all -I$(INC) $(TEST_SRC) -o $(BUILD)/run_tests_asan
	$(BUILD)/run_tests_asan

# ---- Статический анализ ----
lint:
	cppcheck --std=c11 --enable=warning,style,performance,portability \
	         --inline-suppr --error-exitcode=1 --quiet \
	         --suppress=missingIncludeSystem -I$(INC) $(HOST_SRC) tests/run_tests.c
	clang-tidy $(HOST_SRC) tests/run_tests.c -- $(CSTD) -I$(INC)

# clang static analyzer (отдельно, помечает как ошибку)
analyze:
	clang --analyze -Xanalyzer -analyzer-output=text \
	      $(CSTD) -I$(INC) $(HOST_SRC)

# ---- Прошивка (ARM, блоб-исполнитель) ----
fw: $(BUILD)/executor.bin
$(BUILD)/executor.elf: $(ARM_SRC) $(LDSCRIPT)
	$(ARM_CC) $(ARM_FLAGS) -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections \
	    -Wl,-Map=$(BUILD)/executor.map $(ARM_SRC) -o $@
	$(ARM_SIZE) $@
$(BUILD)/executor.bin: $(BUILD)/executor.elf
	$(ARM_OBJ) -O binary $< $@

clean:
	rm -f $(BUILD)/*
