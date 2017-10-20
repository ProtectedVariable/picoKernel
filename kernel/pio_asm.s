global outb
global outw
global inb
global inw

outb:
  enter 0, 0
  mov dx, [ebp+8]
  mov eax, [ebp+12]
  out dx, al
  leave
  ret

outw:
  enter 0, 0
  mov dx, [ebp+8]
  mov eax, [ebp+12]
  out dx, ax
  leave
  ret

inb:
  enter 0, 0
  mov eax, 0
  mov dx, [ebp+8]
  in al, dx
  leave
  ret

inw:
  enter 0, 0
  mov eax, 0
  mov dx, [ebp+8]
  in ax, dx
  leave
  ret
