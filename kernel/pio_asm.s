; PIO assembly file
; Ibanez Thomas, Tournier Vincent

global outb
global outw
global inb
global inw

outb:
  mov WORD dx, [esp+4]
  mov BYTE al, [esp+8]
  out dx, al
  ret

outw:
  mov WORD dx, [esp+4]
  mov WORD ax, [esp+8]
  out dx, ax
  ret

inb:
  mov WORD dx, [esp+4]
  in al, dx
  ret

inw:
  mov WORD dx, [esp+4]
  in ax, dx
  ret
