global _load_idt
_load_idt:
    mov eax, [esp+4]
    lidt [eax]
    ret

%macro isr_noerr 1
global isr%1
isr%1:
    cli
    push 0
    push %1
    jmp isr_stub
%endmacro

%macro isr_err 1
global isr%1
isr%1:
    cli
    push %1
    jmp	isr_stub
%endmacro

isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err 8
isr_noerr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr_noerr 15
isr_noerr 16
isr_noerr 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_noerr 21
isr_noerr 22
isr_noerr 23
isr_noerr 24
isr_noerr 25
isr_noerr 26
isr_noerr 27
isr_noerr 28
isr_noerr 29
isr_noerr 30
isr_noerr 31

extern isr_handler
isr_stub:
    push rax
    push rbx
    push rcx
    push rdi
    push rsi
    push rsp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rsp, rdi

    call isr_handler

    mov rax, rsp

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r9
    pop r8
    pop rsp
    pop rsi
    pop rdi
    pop rcx
    pop rbx
    pop rax

    sti
    iretq