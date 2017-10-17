#include "kernel.h"
#include <util.h>
#include "gdt.h"

void kernelEntry(multiboot_info_t* inf) {
  gdt_init();
  void* frmbf = (void*)0xB8000;
  memset(frmbf, 0, 0xFA0);
}
