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
mov rbx, r12
push rdi
push rsi
push rdx
push rax
push rcx
push r12
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
.convertLoop:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .convertLoop
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r12
pop rcx
pop rax
pop rdx
pop rsi
pop rdi

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
