main: main.o
	gcc -g -Wall -m32 -ansi -o main main.o
main.o: addresses.c
	gcc -g -Wall -m32 -ansi -c -o main.o addresses.c



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main
