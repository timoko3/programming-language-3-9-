section .text
global _start
_start:
mov rbp, rsp
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
push rdi
push rsi
push rdx
push rax
push rcx
push r8
mov rbx, 0
xor r8, r8
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
cmp cl, '-'
jne .read_2
mov r8, 1
jmp .read_0
.read_2:
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .read_0
.readEnd_1:
test r8, r8
jz .readEnd_3
neg rbx
.readEnd_3:
mov r12, rbx
pop r8
pop rcx
pop rax
pop rdx
pop rsi
pop rdi
push rdi
push rsi
push rdx
push rax
push rcx
push r8
mov rbx, 0
xor r8, r8
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
cmp cl, '-'
jne .read_6
mov r8, 1
jmp .read_4
.read_6:
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .read_4
.readEnd_5:
test r8, r8
jz .readEnd_7
neg rbx
.readEnd_7:
mov r13, rbx
pop r8
pop rcx
pop rax
pop rdx
pop rsi
pop rdi
push rdi
push rsi
push rdx
push rax
push rcx
push r8
mov rbx, 0
xor r8, r8
.read_8:
push 0
mov rax, 0
mov rdi, 0
mov rsi, rsp
mov rdx, 1
syscall
test rax, rax
jle .readEnd_9
pop rcx
cmp cl, 10
je .readEnd_9
cmp cl, '-'
jne .read_10
mov r8, 1
jmp .read_8
.read_10:
sub cl, '0'
imul rbx, 10
add rbx, rcx
jmp .read_8
.readEnd_9:
test r8, r8
jz .readEnd_11
neg rbx
.readEnd_11:
mov r14, rbx
pop r8
pop rcx
pop rax
pop rdx
pop rsi
pop rdi
mov rbx, r12
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_12
mov rbx, r13
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_13
mov rbx, r14
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_14
mov rbx, 333
mov r14, rbx


;endAssign
mov rbx, r14
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
xor rbx, rbx
test rax, rax
jns .write_15
neg rax
mov rbx, 1
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
test rbx, rbx
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
.ifEnd_14:
mov rbx, 666
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
xor rbx, rbx
test rax, rax
jns .write_18
neg rax
mov rbx, 1
.write_18:
.write_19:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_19
test rbx, rbx
jz .write_20
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_20:
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
.ifEnd_13:
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
mov r15, rbx


;endAssign
mov rbx, r15
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
xor rbx, rbx
test rax, rax
jns .write_21
neg rax
mov rbx, 1
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
test rbx, rbx
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
.ifEnd_12:
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
sub rsp, 8
mov [rbp - 8], rbx


;endAssign
mov rbx, [rbp - 8]
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jge .ifEnd_24

; sys_exit(0)
mov rax, 60
mov rdi, 0
syscall
.ifEnd_24:
mov rbx, [rbp - 8]
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jne .ifEnd_25
mov rbx, r12
push rdi
mov rdi, rbx
mov rbx, r13
push rsi
mov rsi, rbx
mov rbx, r14
push rdx
mov rdx, rbx
mov rbx, [rbp - 8]
push rcx
mov rcx, rbx
call FUN52
mov rbx, rax
pop rcx
pop rdx
pop rsi
pop rdi


;endCallFunc
.ifEnd_25:
mov rbx, [rbp - 8]
mov r10, rbx
push r10
mov rbx, 0
mov r11, rbx
pop r10
cmp r10, r11
jle .ifEnd_26
mov rbx, r12
push rdi
mov rdi, rbx
mov rbx, r13
push rsi
mov rsi, rbx
mov rbx, r14
push rdx
mov rdx, rbx
mov rbx, [rbp - 8]
push rcx
mov rcx, rbx
call FUN642
mov rbx, rax
pop rcx
pop rdx
pop rsi
pop rdi


;endCallFunc
.ifEnd_26:

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
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor rbx, rbx
test rax, rax
jns .write_27
neg rax
mov rbx, 1
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
test rbx, rbx
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
sub rsp, 1
mov byte [rsp], 10
mov r12, 1
mov rax, rbx
mov rcx, 10
xor rbx, rbx
test rax, rax
jns .write_30
neg rax
mov rbx, 1
.write_30:
.write_31:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_31
test rbx, rbx
jz .write_32
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_32:
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
mov rbx, [rbp - 16]
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
xor rbx, rbx
test rax, rax
jns .write_33
neg rax
mov rbx, 1
.write_33:
.write_34:
mov rdx, 0
div rcx
add rdx, '0'
sub rsp, 1
mov [rsp], dl
add r12, 1
test rax, rax
jnz .write_34
test rbx, rbx
jz .write_35
sub rsp, 1
mov byte [rsp], '-'
add r12, 1
.write_35:
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
