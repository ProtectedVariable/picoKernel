#include "idt.h"
// CPU context used when saving/restoring context from an interrupt
typedef struct regs_st {
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;

static idt_ptr_t idtptr;
static idt_entry_t idt[IDT_ENTRY_COUNT];

// Build and return an IDT entry.
// selector is the code segment selector to access the ISR
// offset is the address of the ISR (for task gates, offset must be 0)
// type indicates the IDT entry type
// dpl is the privilege level required to call the associated ISR
static idt_entry_t idt_build_entry(uint16_t selector, uint32_t offset, uint8_t type, uint8_t dpl) {
	idt_entry_t entry;
	entry.offset15_0 = offset & 0xffff;
	entry.selector = selector;
	entry.reserved = 0;
	entry.type = type;
	entry.dpl = dpl;
	entry.p = 1;
	entry.offset31_16 = (offset >> 16) & 0xffff;
	return entry;
}

// IRQ handler
void irq_handler(regs_t *regs) {
	switch(regs->number) {
		case 0:
			timer_handler();
			break;
		case 1:
			keyboard_handler();
			break;
		default:
			break;
	}
	pic_eoi(regs->number);
}

// Exception handler
void exception_handler(regs_t *regs) {
	setColor(RED);
	switch(regs->number) {
		case 0:
			printf("Divide error");
			break;
		case 1:
			printf("RESERVED");
			break;
		case 2:
			printf("Non-maskable interrupt interrupted");
			break;
		case 3:
			printf("Breakpoint");
			break;
		case 4:
			printf("Overflow");
			break;
		case 5:
			printf("Bound range exceeded");
			break;
		case 6:
			printf("Invalid opcode");
			break;
		case 7:
			printf("Math computing unit not available");
			break;
		case 8:
			printf("Double error");
			break;
		case 9:
			printf("Math processor segfault");
			break;
		case 10:
			printf("Invalid TSS");
			break;
		case 11:
			printf("Segment do not exist");
			break;
		case 12:
			printf("Stack segment fault");
			break;
		case 13:
			printf("General protection - segfault");
			break;
		case 14:
			printf("Page fault");
			break;
		case 15:
			printf("RESERVED");
			break;
		case 16:
			printf("FPU error");
			break;
		case 17:
			printf("Alignment check");
			break;
		case 18:
			printf("Machine check");
			break;
		case 19:
			printf("SIMD floating point exception");
			break;
		case 20:
			printf("Virtualization exception");
			break;
		default:
			break;
	}
	halt();
}

void idt_init() {
	idt[0] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_0, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[1] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_1, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[2] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_2, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[3] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_3, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[4] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_4, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[5] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_5, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[6] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_6, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[7] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_7, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[8] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_8, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[9] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_9, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[10] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_10, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[11] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_11, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[12] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_12, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[13] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_13, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[14] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_14, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[15] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_15, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[16] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_16, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[17] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_code_17, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[18] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_18, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[19] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_19, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[20] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_nocode_20, TYPE_INTERRUPT_GATE, DPL_KERNEL);

	idt[32] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_0, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[33] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_1, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[34] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_2, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[35] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_3, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[36] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_4, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[37] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_5, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[38] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_6, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[39] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_7, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[40] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_8, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[41] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_9, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[42] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_10, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[43] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_11, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[44] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_12, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[45] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_13, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[46] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_14, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt[47] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_15, TYPE_INTERRUPT_GATE, DPL_KERNEL);

	for(int i = 21 ; i < IDT_ENTRY_COUNT ; i++)
	{
		if(i < 32 || i > 47)
			idt[i] = idt_build_entry(0, 0, 0, 0);
	}

	idtptr.base = (int)&idt;
	idtptr.limit = sizeof(idt) - 1;

	idt_load(&idtptr);
}
