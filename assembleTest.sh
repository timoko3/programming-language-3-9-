nasm -f ELF64 in.s
ld   in.o -o in.out
gdb ./in.out