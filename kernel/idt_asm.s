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

%macro  irq_call 1
global _irq_%1
_irq_%1:
	cli          	; disable interrupts
	push    0    	; dummy error code
	push    %1  	; irq number
	jmp		irq_wrapper
%endmacro

irq_call 0
irq_call 1
irq_call 2
irq_call 3
irq_call 4
irq_call 5
irq_call 6
irq_call 7
irq_call 8
irq_call 9
irq_call 10
irq_call 11
irq_call 12
irq_call 13
irq_call 14
irq_call 15


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

;------------------------------------------------
; Wrapper for IRQ

irq_wrapper:
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
    call    irq_handler  ; implemented in idt.c
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
