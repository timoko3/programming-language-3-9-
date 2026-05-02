nasm -f ELF64 test.s
ld   test.o -o test.out
gdb ./test.out