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
	unsigned int memoryAddress = 0; /*the address to load to*/
	char inputFile[20]; 	/*the file to load from*/
	unsigned int location;	/*the location in the file to load from*/
	unsigned int length;/*the length to load*/
	if (sscanf(input, "%X %s %X %d", &memoryAddress, inputFile, &location, &length) != 4) {
		printf("Error: wrong format...");
		exit(1);
	}

	FILE* inFile = fopen(inputFile, "r");
	if (inFile == NULL) {
		printf("Wrong file name!\n");
	}
	fseek(inFile, location, SEEK_SET);
	fread(mem_buffer + memoryAddress, 1, length, inFile);
	printf("Loaded %d units into %p\n", length, &memoryAddress);

}

void saveToFile(char *filename, int size, void *mem_buffer) {
	printf("Please enter <s-address> <t-location> <length>\n");
	char input[20];
	fgets(input, 20, stdin);
	unsigned int s_address;	/*the target file offset in hex*/
	unsigned int t_location;	/*the location in the file to load from*/
	unsigned int length;/*the length to save*/


	/*
		printf("Would you like to save to a different file? <y/other key>\n");
			char ans = fgetc();
		char inputFile[20];
	 different file selection TODO!
		if(ans == 'y'){
			printf("Please enter file name:\n");

		}
		*/


	if (sscanf(input, "%X %X %d", &s_address, &t_location, &length) != 3) {
		printf("Error: wrong format...");
		exit(1);
	}
	FILE* inFile = fopen(fileName, "r+");
	fseek(inFile, t_location, SEEK_SET);
	fwrite(mem_buffer + s_address, 1, length, inFile);
	printf("Saved %d units into %p\n", length, &s_address);
}


void memModify(char *filename, int size, void *mem_buffer) {
	printf("Please enter <address><val>\n");
	char input[20];
	fgets(input, 20, stdin);
	unsigned int address;	/*the target file offset in hex*/
	unsigned int val;	/*the location in the file to load from*/
	char* tmpVal = (char*) &val;

	if (sscanf(input, "%X %X ", &address, &val) != 2) {
		printf("Error: wrong format...");
		exit(1);
	}

	int i;
	for(i = 1; i<=size; i++){
		((char*) mem_buffer)[address + i - 1] = tmpVal[size-i];
	}

	printf("Edit Successfuly Done!\n");
}



struct fun menu[] = {	{"Mem Display", memDisplay},
	{"Load Into Memory", loadToMem},
	{"Save Into File", saveToFile},
	{"Mem Modify", memModify},
	{"Quit", quit}
};
/* ---------------------------------------------------*/





void showMenu() {
	printf("\n\nFile: %s, buffer location: %p, choose action:\n", fileName, buffer);


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
	while (1) {
		showMenu();
		int choice;
		scanf("%d%*c", &choice);
		choice--;
		menu[choice].fun(fileName, size, buffer);
	}

	/*free buffer*/
	free(buffer);
	/************/
	if (file != NULL)
		fclose(file);
	return 0;
}