CC = i386-elf-gcc

CFLAGS 	+= -Wall -fno-builtin -fno-stack-protector -nostdlib
ASFLAGS +=
LDFLAGS += -nostdlib -static

CPPFLAGS +=-nostdinc -g

SRC = $(wildcard *.S) $(wildcard *.c)

BUILD_DIR=./build

# boot.o must be first
OBJS = $(BUILD_DIR)/boot.o
OBJS += $(filter-out $(BUILD_DIR)/boot.o,$(patsubst %.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SRC))))
OBJS += $(patsubst %.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRC)))
DEPS = $(patsubst %.o,%.d,$(OBJS))

bootimg: Makefile $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -Ttext=0x400000 -o img/bootimg
	./makeimage.sh

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c -o $@ $<

$(BUILD_DIR)/%.o: %.S
	$(CC) $(CPPFLAGS) $(ASFLAGS) -MMD -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(BUILD_DIR)/* img/bootimg

-include $(DEPS)
