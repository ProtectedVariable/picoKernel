#include "kernel.h"
#include <util.h>
#include "gdt.h"
#include <display.h>
#include "../common/cursor.h"

void kernelEntry(multiboot_info_t* inf) {
  gdt_init();
  initDisplay();
  //splashScreen();
}
