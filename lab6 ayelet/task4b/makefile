main: main.o LineParser.o
	gcc -g -Wall -m32  -o main main.o LineParser.o -lm
main.o: task4b.c
	gcc -g -Wall -m32 -c  task4b.c -o main.o -lm
LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c LineParser.c -o LineParser.o -lm



#Clean the build directory
clean: 
	rm -f *.o main
