/* Kernel main file
 * Ibanez Thomas, Vincent Tournier
 */
#ifndef __KERNEL__PANIC__
#define __KERNEL__PANIC__
#include <util.h>
#include <cursor.h>
#include <display.h>
#include "gdt.h"
#include "multiboot.h"

/**
 * Entry point of the kernel
 * @param inf grub infomations
 */
extern void kernelEntry(multiboot_info_t* inf);

#endif
