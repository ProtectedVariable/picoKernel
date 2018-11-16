CC=gcc
SRC_FILES=$(wildcard *.c *.s)
OBJ_FILES=$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(SRC_FILES)))
COMPILE_FLAGS=-m32 -nostdlib -ffreestanding -Wall -Wextra $(TEST)
C_COMPILE=$(CC) -c $(COMPILE_FLAGS)
ASM_COMPILE=nasm -f elf32
DEP_FILES=$(OBJ_FILES:.o=.d)
TEST=

all: $(OBJ_FILES)

%.o: %.s
	$(ASM_COMPILE) $< -o $@

%.o: %.c
	$(C_COMPILE) -MMD $< -o $@

clean:
	-rm *.o
	-rm *.d

-include $(DEP_FILES)
