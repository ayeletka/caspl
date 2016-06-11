
ass3:scheduler.o printer.o coroutinesASM.o ass3.o
	gcc -m32 -Wall -g -nostartfiles ass3.o coroutinesC.c scheduler.o printer.o coroutinesASM.o -o ass3

ass3.o:
	nasm -f elf ass3.s -o ass3.o

coroutinesASM.o: coroutinesASM.s
	nasm -f elf coroutinesASM.s -o coroutinesASM.o

scheduler.o: scheduler.s
	nasm -f elf scheduler.s -o scheduler.o

printer.o: printer.s
	nasm -f elf printer.s -o printer.o


clean: 
	rm -f *.o ass3


	
