main: main.o
	gcc -m32 -Wall main.o -o main
	
main.o: main.c
	gcc -m32 -Wall -c main.c -o main.o
	
clean: 
	rm -f *.o main
