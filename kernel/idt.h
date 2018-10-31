/* IDT header file
 * Ibanez Thomas, Tournier Vincent
 */
#ifndef _IDT_H_
#define _IDT_H_

#include "display.h"
#include "x86.h"
#include "pic.h"
#include "keyboard.h"
#include "timer.h"

#define IDT_ENTRY_COUNT 256

/**
 * Strcture to represent the context at the time of interrupt
 */
typedef struct regs_st {
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;

/**
 * Structure for an IDT entry
 */
typedef struct idt_entry_st {
	uint16_t offset15_0;
	uint16_t selector;
	uint16_t reserved : 8;
	uint16_t type : 5;
	uint16_t dpl : 2;
	uint16_t p : 1;
	uint16_t offset31_16;
} __attribute__((packed)) idt_entry_t;

/**
 * Structure to represent the IDT table pointer
 */
typedef struct idt_ptr_st {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

/**
 * Function to load the IDT into the IDTR
 */
void idt_init();

/**
 * Handles an IRQ
 * @þaram regs The context and error code at the moment of the exception
 */
void irq_handler(regs_t *regs);

/**
 * Handles a CPU exception
 * @þaram regs The context and error code at the moment of the exception
 */
void exception_handler(regs_t *regs);

/**
 * 16 functions to handle the 16 IRQs
 */
extern void _irq_0();
extern void _irq_1();
extern void _irq_2();
extern void _irq_3();
extern void _irq_4();
extern void _irq_5();
extern void _irq_6();
extern void _irq_7();
extern void _irq_8();
extern void _irq_9();
extern void _irq_10();
extern void _irq_11();
extern void _irq_12();
extern void _irq_13();
extern void _irq_14();
extern void _irq_15();

/**
 * 16 functions to handle the 16 processor exceptions with or without an error code
 */
extern void _exception_nocode_0();
extern void _exception_nocode_1();
extern void _exception_nocode_2();
extern void _exception_nocode_3();
extern void _exception_nocode_4();
extern void _exception_nocode_5();
extern void _exception_nocode_6();
extern void _exception_nocode_7();
extern void _exception_code_8();
extern void _exception_nocode_9();
extern void _exception_code_10();
extern void _exception_code_11();
extern void _exception_code_12();
extern void _exception_code_13();
extern void _exception_code_14();
extern void _exception_nocode_15();
extern void _exception_nocode_16();
extern void _exception_code_17();
extern void _exception_nocode_18();
extern void _exception_nocode_19();
extern void _exception_nocode_20();

extern void idt_load(idt_ptr_t* idtptr);

#endif
