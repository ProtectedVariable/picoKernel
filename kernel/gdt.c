/* GDT source file
 * Ibanez Thomas, Tournier Vincent
 */
#include "gdt.h"
#define ADDRESSABLE_SPACE 0xFFFFF

static gdt_entry_t gdt[5];
static gdt_ptr_t gdt_ptr;
static tss_t initial_tss;
static uint8_t initial_tss_kernel_stack[TASK_STACK_SIZE];
static task_t tasks[MAX_TASKS];

extern void task_switch(uint16_t tss_selector);
extern void task_ltr(uint16_t tss_selector);

// Build and return a GDT entry.
// base is the base of the segment
// limit is the limit of the segment (NOTE: it's a 20-bit value)
// type is the type of segment
// s indicates whether it's a system segment
// db indicates whether it's a code/data segment or TSS, LDT or gate
// granularity indicates 1 byte or 4KB granularity
// dpl is the privilege level
static gdt_entry_t build_entry(uint32_t base, uint32_t limit, uint8_t type, uint8_t s, uint8_t db, uint8_t granularity, uint8_t dpl) {
	gdt_entry_t entry;
    // For a TSS and LDT, base is the addresse of the TSS/LDT structure
    // and limit is the size of the structure.
    entry.lim15_0 = limit & 0xffff;
    entry.base15_0 = base & 0xffff;
    entry.base23_16 = (base >> 16) & 0xff;
    entry.type = type;  // See TYPE_xxx flags
    entry.s = s;        // 1 for segments; 0 for system (TSS, LDT, gates)
    entry.dpl = dpl;    // privilege level
    entry.present = 1;  // present in memory
    entry.lim19_16 = (limit >> 16) & 0xf;
    entry.avl = 0;      // available for use
    entry.l = 0;        // should be 0 (64-bit code segment)
    entry.db = db;      // 1 for 32-bit code and data segments; 0 for system (TSS, LDT, gate)
    entry.granularity = granularity;  // granularity of the limit value: 0 = 1 byte; 1 = 4096 bytes
    entry.base31_24 = (base >> 24) & 0xff;
	return entry;
}

// Create a NULL "segment".
static gdt_entry_t gdt_make_null_segment() {
	gdt_entry_t entry;
	memset(&entry, 0, sizeof(gdt_entry_t));
	return entry;
}

static gdt_entry_t gdt_make_ldt(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_LDT, S_SYSTEM, DB_SYS, 0, dpl);
}

// Create a code segment specified by the base, limit and privilege level passed in arguments.
static gdt_entry_t gdt_make_code_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_CODE_EXECREAD, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return a data segment specified by the base, limit and privilege level passed in arguments.
static gdt_entry_t gdt_make_data_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_DATA_READWRITE, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

/**
 *
 * @param tss The TSS structure
 * @param dpl The privilege level for the TSS
 * @return A TSS entry specified by the structure and privilege level
 */
static gdt_entry_t gdt_make_tss(tss_t* tss, uint8_t dpl) {
	return build_entry((uint32_t) tss, sizeof(tss_t) - 1, TYPE_TSS, S_SYSTEM, DB_SYS, 0, dpl);
}

/**
 * Transforms a GDT entry in a selector
 * @param entry The GDT entry
 * @return The selector for the GDT entry
 */
static uint gdt_entry_to_selector(gdt_entry_t* entry) {
	return GDT_INDEX_TO_SELECTOR(entry - gdt);
}


static void task_init(uint id) {
	memset(&tasks[id].task_tss, 0, sizeof(tss_t));

	// Add the task's TSS and LDT to the GDT
	gdt[GDT_KERNEL_ENTRIES + id * 2] = gdt_make_tss(&tasks[id].task_tss, DPL_KERNEL);
	gdt[GDT_KERNEL_ENTRIES + id * 2 + 1] = gdt_make_ldt((uint32_t) tasks[id].task_ldt, sizeof(tasks[id].task_ldt) - 1, DPL_KERNEL);
	tasks[id].gdt_tss_sel = gdt_entry_to_selector(&gdt[GDT_KERNEL_ENTRIES + id * 2]);
	tasks[id].gdt_ldt_sel = gdt_entry_to_selector(&gdt[GDT_KERNEL_ENTRIES + id * 2 + 1]);

	// Define code and data segments in the LDT; both segments are overlapping
	tasks[id].address = (void*)(id * TASK_ADDR_SPACE + TASK_START_ADDRESS);
	tasks[id].ldt_code_idx = 0;  // Index of code segment descriptor in the LDT
	tasks[id].ldt_data_idx = 1;  // Index of data segment descriptor in the LDT
	tasks[id].limit = TASK_ADDR_SPACE;  // Limit for both code and data segments
	tasks[id].task_ldt[tasks[id].ldt_code_idx] = gdt_make_code_segment((uint32_t) tasks[id].address, tasks[id].limit / 4096, DPL_USER);
	tasks[id].task_ldt[tasks[id].ldt_data_idx] = gdt_make_data_segment((uint32_t) tasks[id].address, tasks[id].limit / 4096, DPL_USER);

	// Initialize the TSS fields
	// The LDT selector must point to the task's LDT
	tasks[id].task_tss.ldt_selector = tasks[id].gdt_ldt_sel;

	// Setup code and stack pointers
	tasks[id].task_tss.eip = 0;
	tasks[id].task_tss.esp = tasks[id].task_tss.ebp = tasks[id].limit;  // stack pointers

	// Code and data segment selectors are in the LDT
	tasks[id].task_tss.cs = GDT_INDEX_TO_SELECTOR(tasks[id].ldt_code_idx) | DPL_USER | LDT_SELECTOR;
	tasks[id].task_tss.ds = tasks[id].task_tss.es = tasks[id].task_tss.fs = tasks[id].task_tss.gs =
	tasks[id].task_tss.ss = GDT_INDEX_TO_SELECTOR(tasks[id].ldt_data_idx) | DPL_USER | LDT_SELECTOR;
	tasks[id].task_tss.eflags = 512;  // Activate hardware interrupts (bit 9)

	// Task's kernel stack
	tasks[id].task_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	tasks[id].task_tss.esp0 = (uint32_t) (tasks[id].task_kernel_stack) + sizeof(tasks[id].task_kernel_stack);
	tasks[id].free = true;
}

int task_exec(char* filename) {
	int id;
	for (id = 0; id < MAX_TASKS; id++) {
		if (tasks[id].free) {
			break;
		}
	}
	if (id >= MAX_TASKS) {
		return -1;
	}
	int fd = file_open(filename);
	if (fd == -1) {
		printChar('A');
		return -1;
	}
	stat_t stat;
	file_stat(filename, &stat);
	if (file_read(fd, tasks[id].address, stat.size) == -1) {
		printChar('B');
		return -1;
	}
	tasks[id].free = false;
	tasks[id].task_tss.eip = 0;
	tasks[id].task_tss.esp = tasks[id].task_tss.ebp = tasks[id].limit;
	task_switch(tasks[id].gdt_tss_sel);
	tasks[id].free = true;
	return 0;
}

// Initialize the GDT
void gdt_init() {
	gdt[0] = gdt_make_null_segment();
	gdt[1] = gdt_make_code_segment(0, ADDRESSABLE_SPACE, DPL_KERNEL);
	gdt[2] = gdt_make_data_segment(0, ADDRESSABLE_SPACE, DPL_KERNEL);

	gdt_ptr.base = (int)&gdt;
	gdt_ptr.limit = sizeof(gdt) - 1;

    // Load the GDT
    gdt_load(&gdt_ptr);

	gdt[3] = gdt_make_tss(&initial_tss, DPL_KERNEL);
	initial_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	initial_tss.esp0 = ((uint32_t) initial_tss_kernel_stack) + sizeof(initial_tss_kernel_stack);


	task_ltr(gdt_entry_to_selector(&gdt[3]));
	for (uint i = 0; i < MAX_TASKS; ++i) {
		task_init(i);
	}
}
