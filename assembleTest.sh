nasm -f ELF64 circleTest.s
ld   circleTest.o -o circleTest.out
gdb ./circleTest.out