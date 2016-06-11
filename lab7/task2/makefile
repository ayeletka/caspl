hexeditplus: hexeditplus.o
	gcc -g -Wall -m32 -o hexeditplus hexeditplus.o
hexeditplus.o: task3.c
	gcc -g -Wall -m32 -c -o hexeditplus.o task3.c



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o hexeditplus
