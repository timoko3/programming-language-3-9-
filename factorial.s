section .text
global _start
_start:
mov rbx, 0
mov r10, rbx
mov rbx, 0
mov r11, rbx
mov rbx, 0
mov r12, rbx
mov rbx, 1
mov r10, rbx
mov rbx, -5
mov r11, rbx
mov rbx, 6
mov r12, rbx

;startSub

;startMul
mov r8, r11
push r8
mov r9, r11
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r8, rbx
push r8

;startMul

;startMul
mov rbx, 4
mov r8, rbx
push r8
mov r9, r10
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r8, rbx
push r8
mov r9, r12
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r9, rbx
pop r8
sub r8, r9
mov rbx, r8


;endSub
mov r13, rbx

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
