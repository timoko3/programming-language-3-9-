BITS 64

read_int:
    push rdi
    push rsi
    push rdx
    push rcx
    sub rsp, 32

    mov rax, 0          ; read
    mov rdi, 0          ; stdin
    mov rsi, rsp
    mov rdx, 32
    syscall

    xor rax, rax
    xor rcx, rcx

.parse:
    mov bl, [rsp + rcx]

    cmp bl, 10
    je .done

    sub bl, '0'

    imul rax, rax, 10
    add rax, rbx

    inc rcx
    jmp .parse

.done:
    add rsp, 32
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    ret