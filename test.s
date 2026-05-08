section .text
global _start
_start:
mov rbx, 5
mov r12, rbx

;startCallFunc
mov rbx, r12
push rdi
mov rdi, rbx
call FUN472
mov rbx, rax
pop rdi


;endCallFunc
mov r13, rbx

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall


FUN472:
push rbp
mov rbp, rsp
sub rsp, 1
mov [rbp - 1], rbx
mov rbx, 1
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
cmp r10, r11
jge .ifEnd_1

;startSub
mov rbx, rdi
mov r10, rbx
push r10
mov rbx, 1
mov r11, rbx
pop r10
sub r10, r11
mov rbx, r10


;endSub
mov rdi, rbx

;startMul

;startCallFunc
mov rbx, rdi
push rdi
mov rdi, rbx
call FUN472
mov rbx, rax
pop rdi


;endCallFunc
mov r10, rbx
push r10
mov rbx, [rbp - 1]
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10


;endMul
sub rsp, 1
mov [rbp - 1], rbx
mov rbx, [rbp - 1]
mov rax, rbx
add rsp, 1
mov rsp, rbp
pop rbp
ret
.ifEnd_1:
mov rbx, [rbp - 1]
mov rax, rbx
add rsp, 1
mov rsp, rbp
pop rbp
ret
mov rbx, rdi
mov rax, rbx
add rsp, 1
mov rsp, rbp
pop rbp
ret
