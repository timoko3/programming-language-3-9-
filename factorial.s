section .text
global _start
_start:
mov r10, 0
mov r11, r10
mov r10, 0
mov r12, r10
mov r10, 0
mov r13, r10
mov r10, 1
mov r11, r10
mov r10, -5
mov r12, r10
mov r10, 6
mov r13, r10

;startSub

;startMul
mov r8, r12
push r8
mov r9, r12
pop r8
imul r8, r9
mov r10, r8


;endMul
mov r8, r10
push r8

;startMul

;startMul
mov r10, 4
mov r8, r10
push r8
mov r9, r11
pop r8
imul r8, r9
mov r10, r8


;endMul
mov r8, r10
push r8
mov r9, r13
pop r8
imul r8, r9
mov r10, r8


;endMul
mov r9, r10
pop r8
sub r8, r9
mov r10, r8


;endSub
mov r14, r10

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
