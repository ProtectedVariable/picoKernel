%include "const.inc"

section .text   ; start of the text (code) section
align 4         ; the code must be 4 byte aligned

;------------------------------------------------
; CPU exceptions

global _exception_nocode
_exception_nocode:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    xxx  ; exception number
    jmp     exception_wrapper

global _exception_code
_exception_code:
    cli          ; disable interrupts
    push    xxx  ; exception number
    jmp     exception_wrapper

;------------------------------------------------
; IRQ

global _irq
_irq:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    xxx  ; irq number
    jmp     irq_wrapper

;------------------------------------------------
; Wrapper for exceptions

exception_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    exception_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
    
	; Fix the stack pointer due to the 2 push done before the call to
	; exception_wrapper: error code and exception/irq number
    add     esp,8
    iret

