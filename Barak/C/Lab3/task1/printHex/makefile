main: hexaPrint.o
	gcc -m32 -g -Wall -ansi -o main hexaPrint.o
hexaPrint.o: hexaPrint.c
	gcc -m32 -g -Wall -ansi -c -o hexaPrint.o hexaPrint.c
	
clean: 
	rm -f *.o main
