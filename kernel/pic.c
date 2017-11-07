#define PIC1_CMD        0x20
#define PIC1_DATA       0x21
#define PIC2_CMD        0xA0
#define PIC2_DATA       0xA1

// End Of Interrupt (reactivate the specified PIC)
#define PIC_EOI         0x20

// Initialize the PICs by remapping IRQs 0-15 to 32-47
// More details here: http://wiki.osdev.org/8259_PIC
void pic_init() {
	// By default IRQ 0 to 7 (master PIC) are mapped to interrupts 0-7
	// and IRQ 8 to 15 (slave PIC) are mapped to interrupts 8-15.
	// In protected mode, this scheme conflicts with CPU exceptions wich are
	// reserved by the CPU and mapped to interrupts 0 to 31.
	// Therefore, we remap IRQ 0-7 to interrupts 32-39 and
	// IRQ 8-15 to interrupts 40-47

	// Restart both PICs
	outb(PIC1_CMD, 0x11);
	outb(PIC2_CMD, 0x11);

	// Remap IRQ [0..7] to [32..39]
	outb(PIC1_DATA, 32);

	// Remap IRQ [8..15] to [40..47]
	outb(PIC2_DATA, 40);

	// Setup cascading
	outb(PIC1_DATA, 0x04);
	outb(PIC2_DATA, 0x02);
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);
}

// Send an end-of-interrupt to the PICs.
// An EOI must also be sent to the slave for IRQs > 7
void pic_eoi(int irq) {
	if (irq > 7)
		outb(PIC2_CMD, PIC_EOI);
	outb(PIC1_CMD, PIC_EOI);
}

