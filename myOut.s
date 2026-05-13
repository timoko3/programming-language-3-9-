BITS 64

print_int:
    push rsi
    push rcx
    push rdx
    push rdi

    sub rsp, 32

    lea rsi, [rsp + 31]
    mov byte [rsi], 10

    mov rax, rdi
    mov rcx, 10

.convert:
    xor rdx, rdx
    div rcx

    add dl, '0'

    dec rsi
    mov [rsi], dl

    test rax, rax
    jnz .convert

    mov rax, 1
    mov rdi, 1

    lea rdx, [rsp + 32]
    sub rdx, rsi

    syscall

    add rsp, 32

    pop rdi
    pop rdx
    pop rcx
    pop rsi
    ret