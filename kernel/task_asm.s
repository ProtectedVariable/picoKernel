global task_ltr
global task_switch

section .data
tss_sel_offs dd 0  ; must always be 0
tss_sel_seg  dw 0  ; overwritten by the task_switch function

section .text:                     ; start of the text (code) section
align 4                            ; the code must be 4 byte aligned

; Load the task register with the TSS selector passed in argument.
; The TSS selector represents the offset of the TSS descriptor in the GDT table.
; void load_task_register(uint16_t tss_selector);
; NOTE: The GDT must be loaded before issuing the ltr instruction!
;
; void task_ltr(uint16_t tss_selector);
task_ltr:
    mov     eax,[esp+4]
    ltr     ax
    ret

; Call the task specified by the tss selector in argument.
; When the CPU switches to the new task, it automatically loads the task register
; with the new task (ltr instruction) and the LDT from the tss.ldt_selector field.
;
; void task_switch(uint16_t tss_selector)
task_switch:
    mov     ax,[esp+4]  ; get the TSS selector passed in argument (16 bits)
    ; rewrite the segment to jump to with the tss selector passed in argument
	mov		ecx,tss_sel_seg
    mov     [ecx],ax
    call    far [ecx-4]
ret
