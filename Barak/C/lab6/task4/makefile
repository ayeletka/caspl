main: main.o LineParser.o
	gcc -m32 -Wall LineParser.o main.o -o main
	
LineParser.o: LineParser.c
	gcc -m32 -Wall  -c LineParser.c -o LineParser.o

main.o: main.c
	gcc -m32 -Wall -c main.c -o main.o
	
clean: 
	rm -f *.o main
