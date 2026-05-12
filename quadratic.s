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
mov r13, rbx


;endAssign
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
mov r14, rbx


;endAssign
mov rbx, 0
push rdi
push rsi
push rdx
push rax
.read_0:
push 0
mov rax, 0
mov rdi, 0
mov rsi, rsp
mov rdx, 1
syscall
test rax, rax
jle .readEnd_1
pop rcx
cmp cl, 10
je .readEnd_1
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .read_0
.readEnd_1:
mov r12, rbx
mov rbx, 0
push rdi
push rsi
push rdx
push rax
.read_2:
push 0
mov rax, 0
mov rdi, 0
mov rsi, rsp
mov rdx, 1
syscall
test rax, rax
jle .readEnd_3
pop rcx
cmp cl, 10
je .readEnd_3
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .read_2
.readEnd_3:
mov r13, rbx
mov rbx, 0
push rdi
push rsi
push rdx
push rax
.read_4:
push 0
mov rax, 0
mov rdi, 0
mov rsi, rsp
mov rdx, 1
syscall
test rax, rax
jle .readEnd_5
pop rcx
cmp cl, 10
je .readEnd_5
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .read_4
.readEnd_5:
mov r14, rbx
mov rbx, r12
push rdi
mov rdi, rbx
mov rbx, r13
push rsi
mov rsi, rbx
mov rbx, r14
push rdx
mov rdx, rbx
call FUN750
mov rbx, rax
pop rdx
pop rsi
pop rdi


;endCallFunc
mov r15, rbx


;endAssign
mov rbx, r12
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_6
mov rbx, r13
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_7
mov rbx, r14
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_8
mov rbx, r14
mov r10, rbx
push r10
mov rbx, 8
mov r11, rbx
pop r10
add r10, r11
mov rbx, r10
mov r14, rbx


;endAssign
mov rbx, r14
push rdi
push rsi
push rdx
push rax
push rcx
push r12
push r11
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor r11, r11
test rax, rax
jns .write_9
neg rax
mov r11, 1
.write_9:
.write_10:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_10
test r11, r11
jz .write_11
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_11:
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r11
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
.ifEnd_8:
mov rbx, r14
push rdi
push rsi
push rdx
push rax
push rcx
push r12
push r11
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor r11, r11
test rax, rax
jns .write_12
neg rax
mov r11, 1
.write_12:
.write_13:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_13
test r11, r11
jz .write_14
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_14:
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r11
pop r12
pop rcx
pop rax
pop rdx
pop rsi
pop rdi
.ifEnd_7:
mov rbx, -1
mov r10, rbx
push r10
mov rbx, r14
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, r13
mov r11, rbx
pop r10
push rax
push rdx
xor rdx, rdx
mov rax, r10
cqo
idiv r11
mov rbx, rax
pop rdx
pop rax
sub rsp, 8
mov [rbp - 8], rbx


;endAssign
mov rbx, [rbp - 8]
push rdi
push rsi
push rdx
push rax
push rcx
push r12
push r11
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor r11, r11
test rax, rax
jns .write_15
neg rax
mov r11, 1
.write_15:
.write_16:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_16
test r11, r11
jz .write_17
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_17:
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r11
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
.ifEnd_6:
mov rbx, r15
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jge .ifEnd_18

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
.ifEnd_18:
mov rbx, r15
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_19
mov rbx, r12
push rdi
mov rdi, rbx
mov rbx, r13
push rsi
mov rsi, rbx
mov rbx, r14
push rdx
mov rdx, rbx
mov rbx, r15
push rcx
mov rcx, rbx
call FUN52
mov rbx, rax
pop rcx
pop rdx
pop rsi
pop rdi


;endCallFunc
.ifEnd_19:
mov rbx, r15
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jle .ifEnd_20
mov rbx, r12
push rdi
mov rdi, rbx
mov rbx, r13
push rsi
mov rsi, rbx
mov rbx, r14
push rdx
mov rdx, rbx
mov rbx, r15
push rcx
mov rcx, rbx
call FUN642
mov rbx, rax
pop rcx
pop rdx
pop rsi
pop rdi


;endCallFunc
.ifEnd_20:

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall


FUN750:
push rbp
mov rbp, rsp
mov rbx, rsi
mov r10, rbx
push r10
mov rbx, rsi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, 4
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, rdx
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r11, rbx
pop r10
sub r10, r11
mov rbx, r10
sub rsp, 8
mov [rbp - 8], rbx


;endAssign
mov rbx, [rbp - 8]
mov rax, rbx
add rsp, 8
mov rsp, rbp
pop rbp
ret


FUN52:
push rbp
mov rbp, rsp
mov rbx, -1
mov r10, rbx
push r10
mov rbx, rsi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, rcx
mov rbx, rcx
mov r10, rbx
cvtsi2ss xmm0, r10
sqrtss xmm0, xmm0
cvttss2si r10, xmm0
mov rbx, r10
mov r11, rbx
pop r10
add r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, 2
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r11, rbx
pop r10
push rax
push rdx
xor rdx, rdx
mov rax, r10
cqo
idiv r11
mov rbx, rax
pop rdx
pop rax
sub rsp, 8
mov [rbp - 8], rbx


;endAssign
mov rbx, [rbp - 8]
push rdi
push rsi
push rdx
push rax
push rcx
push r12
push r11
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor r11, r11
test rax, rax
jns .write_21
neg rax
mov r11, 1
.write_21:
.write_22:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_22
test r11, r11
jz .write_23
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_23:
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r11
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


FUN642:
push rbp
mov rbp, rsp
mov rbx, -1
mov r10, rbx
push r10
mov rbx, rsi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, rcx
mov rbx, rcx
mov r10, rbx
cvtsi2ss xmm0, r10
sqrtss xmm0, xmm0
cvttss2si r10, xmm0
mov rbx, r10
mov r11, rbx
pop r10
add r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, 2
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r11, rbx
pop r10
push rax
push rdx
xor rdx, rdx
mov rax, r10
cqo
idiv r11
mov rbx, rax
pop rdx
pop rax
sub rsp, 8
mov [rbp - 8], rbx


;endAssign
mov rbx, -1
mov r10, rbx
push r10
mov rbx, rsi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, rcx
mov rbx, rcx
mov r10, rbx
cvtsi2ss xmm0, r10
sqrtss xmm0, xmm0
cvttss2si r10, xmm0
mov rbx, r10
mov r11, rbx
pop r10
sub r10, r11
mov rbx, r10
mov r10, rbx
push r10
mov rbx, 2
mov r10, rbx
push r10
mov rbx, rdi
mov r11, rbx
pop r10
imul r10, r11
mov rbx, r10
mov r11, rbx
pop r10
push rax
push rdx
xor rdx, rdx
mov rax, r10
cqo
idiv r11
mov rbx, rax
pop rdx
pop rax
sub rsp, 8
mov [rbp - 16], rbx


;endAssign
mov rbx, [rbp - 8]
push rdi
push rsi
push rdx
push rax
push rcx
push r12
push r11
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor r11, r11
test rax, rax
jns .write_24
neg rax
mov r11, 1
.write_24:
.write_25:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_25
test r11, r11
jz .write_26
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_26:
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r11
pop r12
pop rcx
pop rax
pop rdx
pop rsi
pop rdi
mov rbx, [rbp - 16]
push rdi
push rsi
push rdx
push rax
push rcx
push r12
push r11
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor r11, r11
test rax, rax
jns .write_27
neg rax
mov r11, 1
.write_27:
.write_28:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_28
test r11, r11
jz .write_29
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_29:
mov rax, 1
mov rdi, 1
mov rsi, rsp
mov rdx, r12
syscall
add rsp, r12
pop r11
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
