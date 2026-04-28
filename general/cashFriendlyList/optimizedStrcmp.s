default rel
section .text

global _start
global optimizedStrcmp

; _start:
;     lea rdi, [str1]
;     lea rsi, [str2]
;     call optimizedStrcmp

optimizedStrcmp:
    vmovdqu8 zmm0, [rdi]
    vmovdqu8 zmm1, [rsi]
    vpxord   zmm2, zmm2, zmm2 

    vpcmpub   k1, zmm0, zmm1, 4
    
    vpcmpub   k2, zmm0, zmm2, 0 
    vpcmpub   k3, zmm1, zmm2, 0 

    kmovq   rax, k2
    tzcnt   rcx, rax

    kmovq   rax, k3
    tzcnt   r8, rax

    cmp rcx, r8
    cmovl rcx, r8

    kmovq   rax, k1
    tzcnt   rdx, rax 

    mov rax, 1d

    cmp rdx, rcx
    jl notEqual
        mov rax, 0d
    notEqual:

    ret

; str1 db "A", 0h
; str2 db "A", 0h

section .note.GNU-stack noalloc noexec nowrite progbits