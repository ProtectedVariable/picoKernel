all:
	mkdir -p picok/boot/grub
	nasm -f elf32 kernel/bootstrap_asm.s -o bootstrap.o
	gcc -c kernel/kernel.c -m32 -o kernel.o
	gcc bootstrap.o kernel.o -Tkernel/kernel.ld -m32 -fno-builtin -ffreestanding -nostdlib -o picok/boot/kernel.elf
	cp grub/menu.lst picok/boot/grub/menu.lst
	cp grub/stage2_eltorito picok/boot/grub/stage2_eltorito
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o picok.iso picok
#qemu-system-i386 -cdrom picok.iso
