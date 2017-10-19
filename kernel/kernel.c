#include "kernel.h"
#include <util.h>
#include "gdt.h"
#include <display.h>
#include "../common/cursor.h"

void kernelEntry(multiboot_info_t* inf) {
  gdt_init();
  clearScreen();
  moveCursor(0, 0);
  printString("pKernel");
  for (int i = 0; i < 10; i++) {
    for (char c = 'A'; c < 'z'; c++) {
      printChar(c);
    }
    setColor(i);
  }
}
