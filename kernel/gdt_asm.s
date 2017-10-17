%include "const.inc"

global gdt_load

section .text:           ; start of the text (code) section

gdt_load:
    mov     eax,[esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt    [eax]        ; Load the new GDT pointer
    mov     ax,GDT_KERNEL_DATA_SELECTOR   ; offset in the GDT of the kernel data segment
    mov     ds,ax        ; Load all data segment selectors
    mov     es,ax
    mov     fs,ax
    mov     gs,ax
    mov     ss,ax
    jmp     GDT_KERNEL_CODE_SELECTOR:.flush  ; far jump [selector:offset]
.flush:
	ret
