#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int flagDebug = 0;
char fileName[100];
size_t unitSize = 2;
unsigned char buffer[4096] = "555";
void Toggle_Debug_Mode();
void Set_File_Name();
void Set_Unit_Size();
void quit();
void memDisplay();
void loadIntoMemory();
void saveIntoFile();
void memModify();

struct fun_desc { /*struct*/
	char *name;
	void (*fun)();
};
struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Set File Name", Set_File_Name }, { "Set Unit Size", Set_Unit_Size},
	{"Load Into Memory", loadIntoMemory}, {"Mem Display", memDisplay} , {"Save Into File", saveIntoFile}, {"Mem Modify", memModify}, { "Quit", quit}, { NULL, NULL }
};

void memModify() {
	char input[100];
	unsigned int s_address = 0;
	unsigned int value;
	char* tmpVal = (char*) &value;
	printf("%s\n", "Please enter <address> in hex, <value> in hex" );
	fgets(input, 100, stdin);
	sscanf(input, "%X %X", &s_address, &value);
	if (flagDebug) {
		fprintf(stderr, "debug:\n s-address: %X\n value: %X\n", s_address, value);
	}
	printf("Moving memory:\n");
	int i;
	for(i = 1; i<=unitSize; i++){
		((char*) buffer)[s_address + i - 1] = tmpVal[unitSize-i];
	}
	printf("Modified buffer.\n");

}

void saveIntoFile() {
	FILE* ft;
	if (fileName == NULL) {
		fprintf(stderr, "%s\n", "file name in null" );
		return;
	}
	else {
		ft = fopen(fileName, "a");
		if (ft == NULL) {
			fprintf(stderr, "%s didn't open\n", fileName);
		}
		char input[100];
		unsigned int s_address = 0;
		unsigned int t_location;
		size_t length;

		printf("%s\n", "Please enter <s-address> in hex, <t-location> in hex,  <length> in dec" );
		fgets(input, 100, stdin);
		sscanf(input, "%X %X %d", &s_address, &t_location, &length);
		if (flagDebug) {
			fprintf(stderr, "debug:\n s-address: %X\n t-location: %X\n length:%d\n", s_address, t_location, length);
		}
		//Check if given location is greater than size:
		fseek(ft, 0, SEEK_END);
		int fileSize = ftell(ft);
		rewind(ft);
		if (t_location > fileSize) {
			fprintf(stderr, "Error: location exceeds file limits.\n");
			return;
		}
		fseek(ft, t_location, SEEK_SET);
		fwrite(buffer + s_address, length, unitSize, ft);
		fclose(ft);

	}
}

void loadIntoMemory() {
	FILE* ft;
	if (fileName == NULL) {
		fprintf(stderr, "%s\n", "file name in null" );
		return;
	}
	else {
		ft = fopen("test", "r");
		if (ft == NULL) {
			fprintf(stderr, "%s\n", "abc.txt didn't open" );
		}
		char input[100];
		unsigned int address ;
		unsigned int location = 0;
		int length;
		printf("%s\n", "Please enter <address> , <location> in hex,  <length> in dec" );
		fgets(input, 100, stdin);
		sscanf(input, "%d %X %d", &address, &location, &length);
		if (flagDebug)
			fprintf(stderr, "debug:\n file name: %s\n mem-adrress: %d\n location:%d\n length:%d\n", fileName, address, location, length);
		if (address == 0) {
			printf("Address 0: loading to buffer...\n");
			printf("Length: %d, unitSize: %d\n", length, unitSize);
			fseek(ft, location, SEEK_SET);
			fread(buffer, length, unitSize, ft);
		}
		fclose(ft);
		printf("Buffer: \n");
		printf("Buffer is: %s\n", buffer);
	}
}

void memDisplay() {
	char input[100];
	unsigned char *address = buffer;
	unsigned int offSet = 0;
	int length;
	printf("%s\n", "Please enter <address> in hex, <length> in dec" );
	fgets(input, 100, stdin);
	sscanf(input, "%X %d", &offSet, &length);
	if (flagDebug) fprintf(stderr, "debug: the default buffer address is %X\n", *address );
	int i;
	address = address + offSet;
	for (i = 0; i < length; i++) {
		printf("%02X ", *(address + i) );
	}
	printf("\n");

}

void Toggle_Debug_Mode() {
	if (flagDebug == 0) {
		flagDebug = 1;
		printf("%s\n", "Debug flag now on");
	}
	else {
		flagDebug = 0;
		printf("%s\n",  "Debug flag now off");
	}
}


void Set_Unit_Size() {
	printf("%s\n", "Please choose the unit size" );
	char input[20];
	int numberOfUnits;
	fgets(input, 20, stdin);
	numberOfUnits = atoi(input);
	if (strcmp(input, "\n") != 0)
	{
		if ((numberOfUnits == 1) || (numberOfUnits == 2) || (numberOfUnits == 2) ) {
			unitSize = numberOfUnits;
			if (flagDebug) fprintf(stderr, "Debug: set size to: %d\n", unitSize );
		}
		else
		{
			fprintf(stderr, "%s\n", "Set_Unit_Size : not within bounds");
		}
	}
}

void Set_File_Name() {
	fflush(stdin);
	printf("insert a file name :\n");
	fgets( fileName, 100, stdin );
	if (flagDebug) fprintf(stderr, "Debug: file name set to: %s\n", fileName );
}

void quit() {
	if (flagDebug) fprintf(stderr, "%s\n", "Debug: quitting" );
	exit(0);
}




int main(int argc, char **argv) {
	int choice;

	char input[20];

	while (true) {
		if (flagDebug) {
			fprintf(stderr, "debug:\n file name: %s\n unit size: %d\n buffer:%X\n", fileName, unitSize, *buffer);
		}
		int i = 0;
		printf("%s\n", "Please choose a function:" );
		while (menu[i].name != NULL && menu[i].fun != NULL) {
			printf("%d) %s\n", i, menu[i].name);
			i++;
		}
		fgets(input, 20, stdin);
		choice = atoi(input);

		if ((0 <= choice) && (choice <= (i - 1)))
			printf("within bounds\n");
		else
		{
			printf("not within bounds\n");
			quit();
		}
		(menu[choice].fun)();


	}
}
