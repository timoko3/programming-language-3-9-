section .text
global _start
_start:
mov rbx, 9
mov r10, rbx
push r10
mov rbx, 3
mov r10, rbx
push r10
mov rbx, 3
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r11, rbx
pop r10
sub r10, r11
mov rbx, r10
mov r12, rbx


;endAssign
mov rbx, 0
push rdi
push rsi
push rdx
push rax
.readLoop:
push 0
mov rax, 0
mov rdi, 0
mov rsi, rsp
mov rdx, 1
syscall
test rax, rax
jle .doneRead
pop rcx
cmp cl, 10
je .doneRead
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .readLoop
.doneRead:
mov r12, rbx

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
