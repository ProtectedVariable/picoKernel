global syscall
section .text                      ; start of the text (code) section
align 4                            ; the code must be 4 byte aligned
; int syscall(uint32_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
syscall:
    ; parameters cannot be passed into the stack because the trap/interrupt gate
    ; performs a stack switch (from user stack to kernel stack). By the time we're
    ; in the syscall handler we're accessing the kernel stack (tss.ss/tss.esp).
    push    ebp
    mov     ebp,esp
	; save all general registers since we modify them below
	; eax is not saved as it's used to store the syscall's return value 
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    mov     eax,[ebp+8]
    mov     ebx,[ebp+12]
    mov     ecx,[ebp+16]
    mov     edx,[ebp+20]
    mov     esi,[ebp+24]
    int     48
   
	; restore all general registers 
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    mov     esp,ebp
    pop     ebp
    ret
