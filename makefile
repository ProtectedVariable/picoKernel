SUBDIRS := $(wildcard kernel/. tools/.)
BUILD_DIR := picok
GRUB_DIR := $(BUILD_DIR)/boot/grub
BOOT_DIR := $(BUILD_DIR)/boot
TEST=

all: picok.iso fs.img

test: TEST=-DTEST
test: run

run: all
	tools/fs_info fs.img
	qemu-system-i386 -cdrom picok.iso -drive file=fs.img,index=0,media=disk,format=raw

$(BOOT_DIR)/kernel.elf: kernel/kernel.elf
	cp kernel/kernel.elf $(BOOT_DIR)/kernel.elf

picok.iso: $(BUILD_DIR) $(SUBDIRS) $(GRUB_DIR)/menu.lst $(GRUB_DIR)/stage2_eltorito $(BOOT_DIR)/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o picok.iso picok

fs.img: splashscreen LargeFile VeryLargeFile
	tools/fs_create picoFS 512 fs.img 1000 4096
	tools/fs_add splashscreen fs.img
	tools/fs_add LargeFile fs.img
	tools/fs_add VeryLargeFile fs.img

$(BUILD_DIR):
	mkdir -p picok/boot/grub

$(GRUB_DIR)/%: grub/%
	cp $< $@

$(SUBDIRS):
	$(MAKE) -C $@ TEST=$(TEST)

clean:
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir clean; \
    done
	-rm picok.iso -f
	-rm -r picok -f
	-rm fs.img -f

.PHONY: all clean test
