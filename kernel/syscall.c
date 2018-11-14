#include "../common/syscall_nb.h"
#include "../common/picofs.h"
#include "../common/types.h"
#include "display.h"
#include "keyboard.h"
#include "filesystem.h"
#include "timer.h"
#include "gdt.h"
// To avoid warnings if an argument is not used.
// Example:
// void f(int arg) {
//     UNUSED(arg);
// }
#define UNUSED(x) ((void)(x))

static  int syscall_exec(char* filename) {
	return task_exec(filename);
}

static int syscall_keypressed() {
	return keypressed();
}

static int syscall_getc() {
	return getc();
}

static int syscall_file_stat(char* filename, stat_t* stat) {
	return file_stat(filename, stat);
}

static int syscall_file_open(char* filename) {
	return file_open(filename);
}

static int syscall_file_close(int fd) {
	file_close(fd);
	return 0;
}

static int syscall_file_read(int fd, char* buf, uint count) {
	return file_read(fd, buf, count);
}

static int syscall_file_seek(int fd, uint offset) {
	return file_seek(fd, offset);
}

static int syscall_file_iterator(file_iterator_t* iterator) {
	file_iterator_t it = file_iterator();
	memcpy(iterator, &it, sizeof(file_iterator_t));
	return 0;
}

static int syscall_file_next(char* filename, file_iterator_t* iterator) {
	file_next(filename, iterator);
	return 0;
}

static int syscall_get_ticks() {
	return get_ticks();
}

static int syscall_sleep(uint ms) {
	sleep(ms);
	return 0;
}

static int syscall_puts(char* string) {
	printString(string);
	return 0;
}

// System call handler: call the appropriate system call according to the nb argument.
// Called by the assembly code _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t caller_tss_selector) {

	void* address = (void*) (TASK_ADDR_SPACE * (SELECTOR_TO_GDT_INDEX(caller_tss_selector) - GDT_KERNEL_SIZE) / 2 + TASK_START_ADDRESS);

    switch (nb) {
		case SYSCALL_PUTS:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_puts(address + arg1);
		case SYSCALL_EXEC:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_exec(address + arg1);
		case SYSCALL_KEYPRESSED:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_keypressed();
		case SYSCALL_GETC:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_getc();
		case SYSCALL_FILE_STAT:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_stat(address + arg1, address + arg2);
		case SYSCALL_FILE_OPEN:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_open(address + arg1);
		case SYSCALL_FILE_CLOSE:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_close(arg1);
		case SYSCALL_FILE_READ:
			UNUSED(arg4);
			return syscall_file_read(arg1, address + arg2, arg3);
		case SYSCALL_FILE_SEEK:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_seek(arg1, arg2);
		case SYSCALL_FILE_ITERATOR:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_iterator(address + arg1);
		case SYSCALL_FILE_NEXT:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_next(address + arg1, address + arg2);
		case SYSCALL_GET_TICKS:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_get_ticks();
		case SYSCALL_SLEEP:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_sleep(arg1);
	}
    return -1;
}
