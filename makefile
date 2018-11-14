SUBDIRS := $(wildcard tools/. user/. common/. kernel/.)
BUILD_DIR := picok
GRUB_DIR := $(BUILD_DIR)/boot/grub
BOOT_DIR := $(BUILD_DIR)/boot
TEST=
APP_BUILD_DIR=build/user/

all: picok.iso fs.img

test: TEST=-DTEST
test: clean run

run: all
	tools/fs_info fs.img
	qemu-system-i386 -cdrom picok.iso -drive file=fs.img,index=0,media=disk,format=raw

$(BOOT_DIR)/kernel.elf: $(SUBDIRS)
	cp kernel/kernel.elf $(BOOT_DIR)/kernel.elf

picok.iso: $(BUILD_DIR) $(GRUB_DIR)/menu.lst $(GRUB_DIR)/stage2_eltorito $(BOOT_DIR)/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o picok.iso picok

fs.img: splashscreen LargeFile VeryLargeFile
	tools/fs_create picoFS 1024 fs.img 1000 4096
	tools/fs_add LargeFile fs.img
	tools/fs_add VeryLargeFile fs.img
	tools/fs_add $(APP_BUILD_DIR)shell fs.img
	tools/fs_add $(APP_BUILD_DIR)app fs.img
	tools/fs_add splashscreen fs.img


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
	-rm -r build -f
	-rm fs.img -f

.PHONY: all clean test $(SUBDIRS)
