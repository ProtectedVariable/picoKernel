#ifndef _X86_H_
#define _X86_H_

// Privilege levels
#define DPL_USER    0x3
#define DPL_KERNEL  0x0

// Selectors
#define LDT_SELECTOR    0x4

// Descriptor types for code and data segments
#define TYPE_DATA_READONLY     1
#define TYPE_DATA_READWRITE    3
#define TYPE_CODE_EXECONLY     9
#define TYPE_CODE_EXECREAD    11

// Descriptor types for system segments and gates
#define TYPE_LDT               2
#define TYPE_TASK_GATE         5
#define TYPE_TSS               9
#define TYPE_CALL_GATE         12
#define TYPE_TRAP_GATE         15
#define TYPE_INTERRUPT_GATE    14

// Descriptor system bit (S)
// For code or data segments
#define S_CODE_OR_DATA   1
// For TSS segment, LDT, call gate, interrupt gate, trap gate, task gate
#define S_SYSTEM   0

// D/B bit
#define DB_SEG  1
#define DB_SYS  0

// kernel code and data selectors in the GDT
#define GDT_KERNEL_CODE_SELECTOR  0x08
#define GDT_KERNEL_DATA_SELECTOR  0x10

// Disable hardware interrupts.
static inline void cli() {
    asm volatile("cli");
}

// Enable hardware interrupts.
static inline void sti() {
    asm volatile("sti");
}

// Halt the processor.
// External interrupts wake up the CPU, hence the cli instruction.
static inline void halt() {
	while (1) asm volatile("cli\nhlt");
}

#endif
