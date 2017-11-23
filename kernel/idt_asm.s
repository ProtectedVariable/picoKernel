/* IDT Assembly file
 * Ibanez Thomas, Tournier Vincent
 */
%include "const.inc"

extern exception_handler
extern irq_handler

section .text   ; start of the text (code) section
align 4         ; the code must be 4 byte aligned

;------------------------------------------------
; CPU exceptions

%macro exception_nocode_call 1
global _exception_nocode_%1
_exception_nocode_%1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    %1  ; exception number
    jmp     exception_wrapper
%endmacro

%macro exception_code_call 1
global _exception_code_%1
_exception_code_%1:
    cli          ; disable interrupts
    push    %1   ; exception number
    jmp     exception_wrapper
%endmacro

exception_nocode_call 0
exception_nocode_call 1
exception_nocode_call 2
exception_nocode_call 3
exception_nocode_call 4
exception_nocode_call 5
exception_nocode_call 6
exception_nocode_call 7
exception_code_call 8
exception_nocode_call 9
exception_code_call 10
exception_code_call 11
exception_code_call 12
exception_code_call 13
exception_code_call 14
exception_nocode_call 15
exception_nocode_call 16
exception_code_call 17
exception_nocode_call 18
exception_nocode_call 19
exception_nocode_call 20

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

global idt_load
idt_load:
    mov     eax,[esp+4]  ; Get the pointer to the IDT, passed as a parameter.
    lidt    [eax]        ; Load the IDT
	ret
