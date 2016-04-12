
all: task0

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
task0: util.o main.o start.o
	ld -melf_i386 start.o main.o util.o -o task0

# Depends on the source and header files
main.o: main.c
	gcc -g -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

util.o: util.c
	gcc -g -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

 
start.o: start.s
	nasm -f elf start.s -o start.o


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task0
