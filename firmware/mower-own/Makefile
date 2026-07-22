# Makefile — mower-own STANDALONE firmware (GD32F305, Cortex-M4, soft-float).
#
# Build inside the mower-fwbuild toolchain container:
#   docker run --rm -v /Users/denis/MOWER/firmware/mower-own:/w mower-fwbuild \
#     bash -c 'cd /w && make'
#
# Produces build/app.elf and build/app.bin. The host-side packer then places
# app.bin at 0x08018000, pads to 0x08100000 with 0xFF, and writes the app CRC-32
# at 0x080FFFFC (tools/bench/app_crc.py).

CROSS   ?= arm-none-eabi-
CC      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
SIZE    := $(CROSS)size

BUILD   := build
LDSCRIPT := ld/app.ld

# EXTRA — extra -D flags for the two isolated bench test images (kept separate so
# results never mix):
#   make EXTRA="-DSPI_WRITE_VERIFY=1"   Test A: factory A4963 write-verify-retry
#   make EXTRA="-DBMS_KEEPALIVE=1"      Test B: full pack keep-alive (CE/C1/C3/53)
# With no EXTRA both features default OFF = the previous behaviour.
EXTRA   ?=

# Flags — mirror the rc-payload conventions (soft-float, freestanding, -Werror).
CFLAGS  := -mcpu=cortex-m4 -mthumb -mfloat-abi=soft \
           -std=c11 -Os -ffreestanding -fno-common \
           -ffunction-sections -fdata-sections \
           -Wall -Wextra -Werror \
           -Isrc $(EXTRA)

LDFLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=soft \
           -T $(LDSCRIPT) -nostartfiles \
           -Wl,--gc-sections -Wl,-Map=$(BUILD)/app.map

SRCS := src/startup.c src/hal.c src/motor.c src/button.c src/battery.c src/bms.c src/main.c
OBJS := $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS))

ELF  := $(BUILD)/app.elf
BIN  := $(BUILD)/app.bin

.PHONY: all clean size

all: $(BIN) size

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS) $(LDSCRIPT) | $(BUILD)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

size: $(ELF)
	$(SIZE) $(ELF)

clean:
	rm -rf $(BUILD)
