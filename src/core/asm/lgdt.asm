global load_gdt
extern kgdtr

section .text
load_gdt:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call next


next:
    ret