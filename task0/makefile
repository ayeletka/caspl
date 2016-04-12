# All Targets
all: ayelet

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
ayelet: numbers.o main.o add.o
	gcc -m32 -g -Wall -o ayelet numbers.o main.o add.o

# Depends on the source and header files
main.o: main.c
	gcc -g -Wall -m32 -ansi -c -o main.o main.c 

numbers.o: numbers.c
	gcc -g -Wall -m32 -ansi -c -o numbers.o numbers.c 
 
add.o: add.s
	nasm -g -f elf -w+all -o add.o add.s


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o ayelet