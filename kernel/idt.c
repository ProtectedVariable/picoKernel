// CPU context used when saving/restoring context from an interrupt
typedef struct regs_st {
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;

static idt_ptr_st idtptr;
static idt_entry_t idt[256];

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

// Exception handler
void exception_handler(regs_t *regs) {

}

void idt_init() {
	idt = (idt_entry_t *) idtptr.base;
	for(int i = 0 ; i < idtptr.limit ; i++)
	{
		if(i < 21)
			idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, 0, TYPE_INTERRUPT_GATE, DPL_KERNEL);
		else if(i >= 32 && i <= 47)
			idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, 0, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	}
}
