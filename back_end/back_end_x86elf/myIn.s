BITS 64

read_int:
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi

    sub rsp, 32

    mov rax, 0
    mov rdi, 0
    mov rsi, rsp
    mov rdx, 32
    syscall

    xor rax, rax
    xor rcx, rcx
    xor rbx, rbx

    mov dl, [rsp]

    cmp dl, '-'
    jne .parse

    mov bl, 1
    inc rcx

.parse:
    mov dl, [rsp + rcx]

    cmp dl, 10
    je .done

    sub dl, '0'

    imul rax, rax, 10

    movzx rdx, dl
    add rax, rdx

    inc rcx
    jmp .parse

.done:
    test rbx, rbx
    jz .exit

    neg rax

.exit:
    add rsp, 32

    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx

    ret