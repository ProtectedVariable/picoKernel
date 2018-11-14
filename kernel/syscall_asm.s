%include "const.inc"
section .text                      ; start of the text (code) section
align 4                            ; the code must be 4 byte aligned
extern syscall_handler
global _syscall_handler
_syscall_handler:
    ; Save all registers
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
    push    eax
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax
    pop     eax
    ; Pass the 6 arguments (nb, arg1, etc.) to the syscall_handler
    ; They are in reverse order to match gcc's IA-32 ABI.
    ; use edi (last arg) to store the task's TSS selector the syscall originated
    ; from, so the kernel can properly address the memory associated to the calling task
    ; (by applying the right offset to addresses passed in arguments)
    str     edi  ; store the segment selector from the task register (TR) in edi
    push    edi
    push    esi
    push    edx
    push    ecx
    push    ebx
    push    eax
    call    syscall_handler
    ; These 6 "pop eax" instructions are only here to balance the pushes
    ; above used to pass the arguments to the syscall_handler function
    pop     ebx
    pop     ebx
    pop     ebx
    pop     ebx
    pop     ebx
    pop     ebx
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
    iret
