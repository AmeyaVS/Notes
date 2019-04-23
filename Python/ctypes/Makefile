PROJECT := clib

CC := gcc

CFLAGS := -MMD -MP -Wall -Wextra -Wpedantic -fPIC

# For out of source build
BUILD_DIR := build
MKDIR_P := mkdir -p
RM := rm -rfv

SRCS_C := clib.c
OBJS := $(SRCS_C:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(BUILD_DIR)/lib$(PROJECT).so
	@echo "===================="
	@echo "Running with Python2"
	python clib.py
	@echo "===================="
	@echo "Running with Python3"
	python3 clib.py


$(BUILD_DIR)/lib$(PROJECT).so: $(BUILD_DIR)/clib.c.o
	$(CC) -shared -o $@ $^

$(BUILD_DIR)/%.c.o: %.c
	@if [ ! -d "$(dir $@)" ]; then $(MKDIR_P) $(dir $@); fi
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM) $(BUILD_DIR)/lib$(PROJECT).so $(OBJS) $(DEPS)

-include $(DEPS)
