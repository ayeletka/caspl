mypipe: mypipe.o
	gcc -g -Wall -m32  -o mypipe mypipe.o -lm
mypipe.o: task2.c
	gcc -g -Wall -m32 -c  task2.c -o mypipe.o -lm



#Clean the build directory
clean: 
	rm -f *.o mypipe
