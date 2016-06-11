main: main.o
	gcc -g -Wall -m32  -o main main.o
main.o: main.c
	gcc -g -Wall -m32  -c -o main.o main.c



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main
