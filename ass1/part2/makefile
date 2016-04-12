CC		:=	gcc 
CC_FLAGS	:=	-m32
ASM		:=	nasm
ASM_FLAGS	:=	-f elf 



all: task2 

task2:	
	$(ASM) $(ASM_FLAGS)  task2.s -o task2.o
	$(CC) $(CC_FLAGS) main2.c  task2.o -o task2.bin
	
# task2:
# add your makefile code here...
# uncomment task2 in section 'all'


