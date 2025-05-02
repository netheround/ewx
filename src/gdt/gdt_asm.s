section .text
global _load_gdt

_load_gdt:
    lgdt [rdi]             ; Load GDT using pointer in RDI

    mov ax, 0x40           ; TSS selector
    ltr ax                 ; Load Task Register

    mov ax, 0x10           ; Kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far return to reload CS
    ; Stack still contains return address pushed by `call`
    ; So we just push the CS and do an lret

    mov rax, 0x08          ; Kernel code segment
    push rax               ; Push new CS
    push qword [rsp]       ; Duplicate return address
    add rsp, 8             ; Remove old return address (we replaced it)
    lretq                  ; Far return
