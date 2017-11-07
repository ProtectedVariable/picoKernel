FLAGS=-fno-builtin -ffreestanding -nostdlib

all: gdt_asm.o bootstrap.o kernel.o gdt.o pio.o

test: FLAGS += -DTEST
test: test.o all
	@echo "Test version"

run: all

test.o: test.c test.h
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c test.c -o test.o

gdt_asm.o: gdt_asm.s
	nasm -f elf32 gdt_asm.s -o gdt_asm.o

pio.o: pio_asm.s pio.h
	nasm -f elf32 pio_asm.s -o pio.o

bootstrap.o: bootstrap_asm.s
	nasm -f elf32 bootstrap_asm.s -o bootstrap.o

kernel.o: kernel.h multiboot.h kernel.c
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c kernel.c -o kernel.o

gdt.o: gdt.h x86.h ../common/types.h gdt.c
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c gdt.c -o gdt.o

clean:
	-rm *.o -f
