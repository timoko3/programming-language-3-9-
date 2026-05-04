section .text
global _start
_start:
mov rbx, 0
mov r12, rbx
mov rbx, 0
mov r13, rbx
mov rbx, 0
mov r14, rbx
mov rbx, 1
mov r12, rbx
mov rbx, -5
mov r13, rbx
mov rbx, 6
mov r14, rbx

;startSub

;startMul
mov rbx, r13
mov r10, rbx
push r10
mov rbx, r13
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r10, rbx
push r10

;startMul

;startMul
mov rbx, 4
mov r10, rbx
push r10
mov rbx, r12
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r10, rbx
push r10
mov rbx, r14
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r11, rbx
pop r10
sub r10, r11
mov rbx, r10


;endSub
mov r15, rbx

;startCallFunc
mov rbx, r12
mov rdi, rbx
mov rbx, r13
mov rsi, rbx
mov rbx, r14
mov rdx, rbx
mov rbx, r15
mov rcx, rbx
call FUN1642


;endCallFunc

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall


FUN2642:
push rbp
mov rbp, rsp

;startDiv

;startAdd

;startMul
mov rbx, -1
mov r10, rbx
push r10
mov rbx, rsi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r10, rbx
push r10

;startSqrt
mov rbx, rcx
mov r10, rbx
cvtsi2ss xmm0, r10
sqrtss xmm0, xmm0
cvttss2si r10, xmm0
mov rbx, r10


;endSqrt
mov r11, rbx
pop r10
add r10, r11
mov rbx, r10


;endAdd
mov r10, rbx
push r10

;startMul
mov rbx, 2
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r11, rbx
pop r10
push rax
push rdx
xor rdx, rdx
mov rax, r10
idiv r11
mov rbx, rax
pop rdx
pop rax


;endDiv
sub esp, 8
mov [rbp - 8], rbx

;startDiv

;startSub

;startMul
mov rbx, -1
mov r10, rbx
push r10
mov rbx, rsi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r10, rbx
push r10

;startSqrt
mov rbx, rcx
mov r10, rbx
cvtsi2ss xmm0, r10
sqrtss xmm0, xmm0
cvttss2si r10, xmm0
mov rbx, r10


;endSqrt
mov r11, rbx
pop r10
sub r10, r11
mov rbx, r10


;endSub
mov r10, rbx
push r10

;startMul
mov rbx, 2
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
mov r11, rbx
pop r10
push rax
push rdx
xor rdx, rdx
mov rax, r10
idiv r11
mov rbx, rax
pop rdx
pop rax


;endDiv
sub esp, 8
mov [rbp - 16], rbx

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
add rsp, 16
mov rsp, rbp
pop rbp
ret
