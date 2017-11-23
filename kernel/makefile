CC=gcc
SRC_FILES=$(wildcard *.c *.s)
OBJ_FILES=$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(SRC_FILES)))
COMPILE_FLAGS=-m32 -nostdlib -ffreestanding -Wall -Wextra $(TEST)
LINKER_FLAGS=-Tkernel.ld -static -lgcc
C_COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS) $(LINKER_FLAGS)
ASM_COMPILE=nasm -f elf32
DEP_FILES=$(OBJ_FILES:.o=.d)
TEST=

kernel.elf: kernel.ld $(OBJ_FILES)
	$(LINK) $(OBJ_FILES) -o $@

%.o: %.s
	$(ASM_COMPILE) $< -o $@

%.o: %.c
	$(C_COMPILE) -MMD $< -o $@

clean:
	-rm *.o

-include $(DEP_FILES)
