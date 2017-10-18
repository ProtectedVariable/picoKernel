SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)
	mkdir -p picok/boot/grub
	gcc common/cursor.o common/display.o common/util.o kernel/bootstrap.o kernel/kernel.o kernel/gdt.o kernel/gdt_asm.o -Icommon -Tkernel/kernel.ld -m32 -fno-builtin -ffreestanding -nostdlib -o picok/boot/kernel.elf
	cp grub/menu.lst picok/boot/grub/menu.lst
	cp grub/stage2_eltorito picok/boot/grub/stage2_eltorito
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o picok.iso picok
	qemu-system-i386 -cdrom picok.iso
$(SUBDIRS):
	-$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
