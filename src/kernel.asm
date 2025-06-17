[BITS 32]

global _start

extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10
_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Remap the master PIC
    mov al, 00010001b   ; Put PIC in initialization mode
    out 0x20, al    ; Tell master PIC

    mov al, 0x20    ; Interrupt 0x20 is where master ISR should start
    out 0x21, al    ; Data to master PIC

    mov al, 00000001b   ; Put PIC into x86 mode
    out 0x21, al
    ; End remap of master PIC
    
    call kernel_main
    jmp $

times 512-($-$$) db 0