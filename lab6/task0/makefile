mypipe: mypipe.o
	gcc -g -Wall -m32  -o mypipe mypipe.o -lm
mypipe.o: mypipe.c
	gcc -g -Wall -m32 -c  mypipe.c -o mypipe.o -lm



#Clean the build directory
clean: 
	rm -f *.o mypipe