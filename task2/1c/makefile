main: main.o
	gcc -g -o0 -Wall -m32 -ansi -o main main.o
main.o: 1c.c
	gcc -g -o0 -Wall -m32 -ansi -c -o main.o 1c.c



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main
