#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int size = 1;
char* fileName;
void* buffer;
FILE* file = NULL;
int bufferLocation = 2;

struct fun {
	char *name;
	void (*fun)(char *filename, int size, void *mem_buffer);
};

void quit(char *filename, int size, void *mem_buffer) {
	fclose(file);
	printf("Quit...\n");
	exit(0);
}

void memDisplay(char *filename, int size, void *mem_buffer) {
	printf("Please enter <address> <length>\n");
	char input[20];
	fgets(input, 20, stdin);
	unsigned char* buf = mem_buffer;
	unsigned int offset = 0;
	int length;
	if (sscanf(input, "%X %d", &offset, &length) != 2) {
		printf("Error: wrong format...\n");
	}
	buf += offset;
	while (length > 0) {
		int i;
		for (i = 0; i < size; i++) {
			printf("%02X", *buf++);
		}
		printf(" ");
		length--;
	}
	printf("\n");
}


void loadToMem(char *filename, int size, void *mem_buffer) {
	printf("Please enter <mem-address> <source-file> <location> <length>\n");
	char input[20];
	fgets(input, 20, stdin);
	unsigned int memoryAddress = 0;
	char* inputFile = 0;
	unsigned int offset;
	unsigned int length;

	if (sscanf(input, "%X %s %X %d",&memoryAddress, inputFile, &offset, &length) != 4) {
		printf("Error: wrong format...\n");
	}
	FILE* inFile = fopen(fileName, "r+");

	printf("Loaded %d units into %p\n", length, memoryAddress);


}
void saveToMem(char *filename, int size, void *mem_buffer) {
	exit(0);

}
struct fun menu[] = {	{"Mem Display", memDisplay},
	{"Load Into Memory", loadToMem},
	{"Save Into Memory", saveToMem},
	{"Quit", quit}
};
/* ---------------------------------------------------*/





void showMenu() {
	printf("\e[2J\e[H"); /*clean the screen*/
	printf("\n\n    ------------------------------------------------------------------     \n");
	printf("                         ****Hexedit Plus ****\n");
	printf("    ------------------------------------------------------------------     \n\n");
	printf("File: %s, buffer location: %p, choose action:\n", fileName, buffer);


	/*iteratively print the menu:*/
	int sz = sizeof(menu) / sizeof(menu[0]);
	int i = 0;
	for (i = 0; i < sz; i++)
		printf("%d-%s\n", i + 1, menu[i].name);

}

int main (int argc , char* argv[], char* envp[]) {
	/*-------------   get args: -----------------*/
	if (argc == 1) {
		printf("Error: No filename received!\n");
		exit(1);
	}
	fileName = argv[1];
	if (argc == 3) {
		size = atoi(argv[2]);
		printf("Size: %d\n", size);
		if (!(size == 1 || size == 2 || size == 4)) {
			printf("Invalid size!\n");
			exit(1);
		}
	}
	/*-----------------------------------------------*/

	file = fopen(fileName, "r+");
	if (file == NULL) {
		printf("Error Opening the file!\n");
		exit(1);
	}


	buffer = (void*)malloc(1024 * 4);
	showMenu();
	int choice;
	scanf("%d%*c", &choice);
	choice--;
	menu[choice].fun(fileName, size, &buffer);


	/*free buffer*/
		free(buffer);
	/************/
	if (file != NULL)
		fclose(file);
	return 0;
}