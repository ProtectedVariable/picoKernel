FLAGS=-fno-builtin -ffreestanding -nostdlib

all: gdt_asm.o bootstrap.o kernel.o gdt.o pio.o keyboard.o pic.o timer.o idt.o idt_asm.o

test: FLAGS += -DTEST
test: test.o all
	@echo "Test version"

run: all

test.o: test.c test.h
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c test.c -o test.o

gdt_asm.o: gdt_asm.s
	nasm -f elf32 gdt_asm.s -o gdt_asm.o

idt_asm.o: idt_asm.s
	nasm -f elf32 idt_asm.s -o idt_asm.o

pio.o: pio_asm.s pio.h
	nasm -f elf32 pio_asm.s -o pio.o

bootstrap.o: bootstrap_asm.s
	nasm -f elf32 bootstrap_asm.s -o bootstrap.o

pic.o: pic.c pic.h
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c pic.c -o pic.o

keyboard.o: keyboard.c keyboard.h
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c keyboard.c -o keyboard.o

kernel.o: kernel.h multiboot.h kernel.c
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c kernel.c -o kernel.o

gdt.o: gdt.h x86.h ../common/types.h gdt.c
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c gdt.c -o gdt.o

idt.o: idt.h idt.c
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c idt.c -o idt.o

timer.o: timer.h timer.c
	gcc -m32 -Wall -Wextra -I../common -std="gnu11" $(FLAGS) -c timer.c -o timer.o

clean:
	-rm *.o -f
