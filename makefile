SUBDIRS := $(wildcard kernel/.)
BUILD_DIR := picok
GRUB_DIR := $(BUILD_DIR)/boot/grub
BOOT_DIR := $(BUILD_DIR)/boot
TEST=

all: picok.iso

test: TEST=-DTEST
test: run

run: all
	qemu-system-i386 -cdrom picok.iso

$(BOOT_DIR)/kernel.elf: kernel/kernel.elf
	cp kernel/kernel.elf $(BOOT_DIR)/kernel.elf

picok.iso: $(BUILD_DIR) $(SUBDIRS) $(GRUB_DIR)/menu.lst $(GRUB_DIR)/stage2_eltorito $(BOOT_DIR)/kernel.elf $(BOOT_DIR)/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o picok.iso picok

$(BUILD_DIR):
	mkdir -p picok/boot/grub

$(GRUB_DIR)/%: grub/%
	cp $< $@

$(SUBDIRS):
	$(MAKE) -C $@ TEST=$(TEST)

clean:
	$(MAKE) -C $(SUBDIRS) clean
	-rm picok.iso -f
	-rm -r picok -f

.PHONY: all clean test $(SUBDIRS)
