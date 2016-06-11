hex: task1.c task4.c
	gcc -g -m32 -Wall -o hexeditplus task1.c
	gcc -g -m32 -Wall -o task4 task4.c

	

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o hexeditplus
