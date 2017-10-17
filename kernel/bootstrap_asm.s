global entrypoint  ; the entry point symbol defined in kernel.ld

; Values for the multiboot header
MULTIBOOT_MAGIC        equ 0x1BADB002
MULTIBOOT_ALIGN_MODS   equ 1
MULTIBOOT_MEMINFO      equ 2
MULTIBOOT_VIDINFO      equ 4

MULTIBOOT_FLAGS     equ (MULTIBOOT_ALIGN_MODS|MULTIBOOT_MEMINFO)

; Magic + checksum + flags must equal 0!
MULTIBOOT_CHECKSUM  equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

;---------------------------------------------------------------------------------------------------
; .multiboot section
; This section must be located at the very beginning of the kernel image.

section .multiboot

; Mandatory part of the multiboot header
; see http://git.savannah.gnu.org/cgit/grub.git/tree/doc/multiboot.h?h=multiboot
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

entrypoint:
	; code starts executing here
	cli  ; disable hardware interrupts

	mov eax, 0xB87C2 ;middle of the framebuffer minus half of the word
  mov word [eax], 0x0948 ;H
  mov word [eax+2], 0x0965;e
  mov word [eax+4], 0x096C;l
  mov word [eax+6], 0x096C;l
  mov word [eax+8], 0x096F;o
  mov word [eax+12], 0x0957;W
  mov word [eax+14], 0x096F;o
  mov word [eax+16], 0x0972;r
  mov word [eax+18], 0x096C;l
  mov word [eax+20], 0x0964;d
	; TODO :
	; - Initialize the stack pointer and EBP (both to the same value)
	; - Pass the multiboot info to the kernel
	; - Call the kernel entry point (C code)
	; ...

	; infinite loop (should never get here)
.forever:
	hlt
	jmp .forever

;---------------------------------------------------------------------------------------------------
; TODO : declare a .stack section for the kernel. It should at least be 1MB long. Given this stack
; area won't be initialized, the nobits keyword should be added when declaring the section.
; ...
