BITS 64

print_int:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi

    sub rsp, 32

    lea rsi, [rsp + 31]
    mov byte [rsi], 10

    mov rax, rdi
    mov rbx, 10

    xor rcx, rcx

    cmp rax, 0
    jge .convert

    neg rax
    mov rcx, 1

.convert:
    xor rdx, rdx
    div rbx

    add dl, '0'

    dec rsi
    mov [rsi], dl

    test rax, rax
    jnz .convert

    test rcx, rcx
    jz .write

    dec rsi
    mov byte [rsi], '-'

.write:
    mov rax, 1
    mov rdi, 1

    lea rdx, [rsp + 32]
    sub rdx, rsi

    syscall

    add rsp, 32

    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ret