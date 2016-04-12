main: main.o
	gcc -g -Wall -m32 -ansi -o main main.o -lm
main.o: task1a.c
	gcc -g -Wall -m32 -ansi -c -o main.o task1a.c -lm



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main
