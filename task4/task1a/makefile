
all: task1a

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
task1a: util.o filter.o start.o
	ld -melf_i386 start.o filter.o util.o -o task1a

# Depends on the source and header files
filter.o: filter.c
	gcc -m32 -Wall -d -ansi -c -nostdlib -fno-stack-protector filter.c -o filter.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

 
start.o: start.s
	nasm -f elf start.s -o start.o


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1a