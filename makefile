SUBDIRS := $(wildcard common/. kernel/.)
OBJ_FILES := common/cursor.o common/display.o common/util.o kernel/bootstrap.o kernel/pio.o kernel/kernel.o kernel/gdt.o kernel/gdt_asm.o
BUILD_DIR := picok
GRUB_DIR := $(BUILD_DIR)/boot/grub
BOOT_DIR := $(BUILD_DIR)/boot

all: $(BUILD_DIR) $(SUBDIRS) $(GRUB_DIR)/menu.lst $(GRUB_DIR)/stage2_eltorito $(BOOT_DIR)/kernel.elf picok.iso

test: OBJ_FILES += kernel/test.o
test: all

run:
	qemu-system-i386 -cdrom picok.iso

$(BOOT_DIR)/kernel.elf: $(OBJ_FILES)
	gcc $(OBJ_FILES) -Icommon -Tkernel/kernel.ld -m32 -fno-builtin -ffreestanding -nostdlib -o $(BOOT_DIR)/kernel.elf

picok.iso: $(BOOT_DIR)/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o picok.iso picok

$(BUILD_DIR):
	mkdir -p picok/boot/grub

$(GRUB_DIR)/%: grub/%
	cp $< $@

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)
	-rm picok.iso -f
	-rm -r picok -f
test: $(SUBDIRS)

.PHONY: all clean test $(SUBDIRS)
