
all: task0b

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
task0b: util.o patch.o start.o
	ld -melf_i386 start.o patch.o util.o -o task0b

# Depends on the source and header files
patch.o: patch.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector patch.c -o patch.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

 
start.o: start.s
	nasm -f elf start.s -o start.o


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task0b