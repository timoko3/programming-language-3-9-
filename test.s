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
mov r8, r13
push r8
mov rbx, 0
mov r9, rbx
pop r8
cmp r8, r9
jge .ifEnd_1

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
.ifEnd_1:
mov r8, r13
push r8
mov rbx, 0
mov r9, rbx
pop r8
cmp r8, r9
jne .ifEnd_2

;startDiv

;startAdd

;startMul
mov rbx, -1
mov r8, rbx
push r8
mov r9, r11
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r8, rbx
push r8

;startSqrt
mov r8, r13
cvtsi2ss xmm0, r8
sqrtss xmm0, xmm0
cvttss2si r8, xmm0
mov rbx, r8


;endSqrt
mov r9, rbx
pop r8
add r8, r9
mov rbx, r8


;endAdd
mov r8, rbx
push r8

;startMul
mov rbx, 2
mov r8, rbx
push r8
mov r9, r10
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r9, rbx
pop r8
push rax
push rdx
xor rdx, rdx
mov rax, r8
idiv r9
mov rbx, rax
pop rdx
pop rax


;endDiv
mov r14, rbx

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
.ifEnd_2:
mov r8, r13
push r8
mov rbx, 0
mov r9, rbx
pop r8
cmp r8, r9
jle .ifEnd_3

;startDiv

;startAdd

;startMul
mov rbx, -1
mov r8, rbx
push r8
mov r9, r11
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r8, rbx
push r8

;startSqrt
mov r8, r13
cvtsi2ss xmm0, r8
sqrtss xmm0, xmm0
cvttss2si r8, xmm0
mov rbx, r8


;endSqrt
mov r9, rbx
pop r8
add r8, r9
mov rbx, r8


;endAdd
mov r8, rbx
push r8

;startMul
mov rbx, 2
mov r8, rbx
push r8
mov r9, r10
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r9, rbx
pop r8
push rax
push rdx
xor rdx, rdx
mov rax, r8
idiv r9
mov rbx, rax
pop rdx
pop rax


;endDiv
mov r14, rbx

;startDiv

;startSub

;startMul
mov rbx, -1
mov r8, rbx
push r8
mov r9, r11
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r8, rbx
push r8

;startSqrt
mov r8, r13
cvtsi2ss xmm0, r8
sqrtss xmm0, xmm0
cvttss2si r8, xmm0
mov rbx, r8


;endSqrt
mov r9, rbx
pop r8
sub r8, r9
mov rbx, r8


;endSub
mov r8, rbx
push r8

;startMul
mov rbx, 2
mov r8, rbx
push r8
mov r9, r10
pop r8
imul r8, r9
mov rbx, r8


;endMul
mov r9, rbx
pop r8
push rax
push rdx
xor rdx, rdx
mov rax, r8
idiv r9
mov rbx, rax
pop rdx
pop rax


;endDiv
mov r15, rbx

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
.ifEnd_3:

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
