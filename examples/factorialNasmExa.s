section .text

global _start

_start: 
    mov r8, 0d
    mov r9, 5d

    mov rax, r8
    call zavalinka

    mov [printBuffer], rax

    mov rax, 0x01
    mov rdi, 1d
    lea rsi, [printBuffer]
    mov rdx, printBufferLen
    syscall

    mov rax, 60     
    mov rdi, 0      
    syscall
zavalinka:
    mov rcx, 1d

    mov rdx, 1d

    .startCycle:
    cmp rax, rcx
    jle .endCycle
        imul rdx, rcx

        inc rcx
    jmp .startCycle
    .endCycle:

    mov rax, rdx
    ret 

section .data

printBuffer:       db 8 dup(0), 0x0
printBufferLen     equ $ - printBuffer