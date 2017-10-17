all: gdt_asm.o bootstrap.o kernel.o gdt.o

gdt_asm.o: gdt_asm.s
	nasm -f elf32 gdt_asm.s -o gdt_asm.o

bootstrap.o: bootstrap_asm.s
	nasm -f elf32 kernel/bootstrap_asm.s -o bootstrap.o

kernel.o: kernel.h multiboot.h kernel.c
	gcc -m32 -Wall -Wextra -std="gnu11" -c kernel.c -o kernel.o

gdt.o: gdt.h x86.h ../common/types.h gdt.c
	gcc -m32 -Wall -Wextra -std="gnu11" -c gdt.c -o gdt.o
