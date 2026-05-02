section .text
global _start
_start:
	mov rbx, 0
	mov r10, 0
	mov r11, 0
	mov rbx, 1
	mov r10, -5
	mov r11, 6

	; sys_exit(0)
	mov rax, 60
	mov rdi, 0
	syscall
