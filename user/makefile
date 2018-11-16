CC=gcc
TEST_FLAGS=
COMPILE_FLAGS=-m32 -MMD -g -nostdlib -ffreestanding -Wall -Wextra $(TEST_FLAGS) -fno-pie
LINKER_FLAGS=-Tapp.ld
COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS) $(LINKER_FLAGS)
NASM=nasm -g -f elf32
BUILD_FOLDER=../build/user/
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(BUILD_FOLDER)%.o,$(SRCS))
DEPS=$(OBJS:.o=.d)

APP_LIBS=$(BUILD_FOLDER)entrypoint_asm.o $(BUILD_FOLDER)picolib.o $(BUILD_FOLDER)syscall_asm.o $(BUILD_FOLDER)util.o

$(shell mkdir -p $(BUILD_FOLDER))

all: $(BUILD_FOLDER)app $(BUILD_FOLDER)shell

$(BUILD_FOLDER)%: $(BUILD_FOLDER)%.o $(APP_LIBS)
	$(LINK) $^ -o $@

$(BUILD_FOLDER)%.o: %.c
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)%.o: %.s
	$(NASM) $^ -o $@

$(BUILD_FOLDER)util.o: ../common/util.c ../common/util.h
	$(COMPILE) $< -o $@

clean:
	@-rm -r -f $(BUILD_FOLDER)

-include $(DEPS)
