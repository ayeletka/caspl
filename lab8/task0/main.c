#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#define YELLOW "\x1B[33m"
#define CYAN "\x1B[36m"
#define NATURAL   "\x1B[0m"

int flagDebug = 0;
char fileName[100];
struct stat fd_stat; //keep the size of the file
void *map_start; // will point to the start of the memory mapped file
 Elf32_Ehdr *header; // this will point to the header structure
 int fd;
void Toggle_Debug_Mode();
void examine_elf_file();
void Set_Unit_Size();
void quit();


struct fun_desc { /*struct*/
	char *name;
	void (*fun)();
};
struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Examine elf file", examine_elf_file }, { "Quit", quit}, { NULL, NULL }
};

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


void examine_elf_file() {
	fflush(stdin);
	printf("insert a file name :\n");
	scanf("%s", fileName);
	//fgets( fileName, 100, stdin );
		fflush(stdin);

	if (flagDebug) fprintf( stderr,CYAN "Debug: file name set to: %s\n"NATURAL, fileName );

	
	if (fileName == NULL) {
		fprintf(stderr, "%s\n", "file name in null" );
		return;
	}
	else {
		fd = open(fileName, O_RDONLY); //TODO: change to fileName
		if (fd <0) {
			fprintf(stderr, "%s didn't open\n", "ntsc" ); //TODO: change to fileName
		}
		if (fstat(fd, &fd_stat) != 0 ) {
	      perror("stat failed");
	      exit(-1);
	   }
	   if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ , MAP_PRIVATE, fd, 0)) == MAP_FAILED ) {
	      perror("mmap failed");
	      exit(-1);
	   }
	   header = (Elf32_Ehdr *) map_start;

		printf("The maggic number: %.3s\n",(char*)header+1);
		if(strncmp((char*)header->e_ident,ELFMAG,3 )==0)
			printf("%s\n","yesss" );
		else {
			printf("%s\n","maggic number is incorrect" );
			quit();
		}
		printf(YELLOW"%-50s%x\n","Data encoding:", header->e_ident[EI_DATA]);
		printf("%-50s%x\n","Entry Point:", header->e_entry);
		printf("%-50s%x\n","The file Offset for Header table:", header->e_shoff);
		printf("%-50s%x\n","Number of Header entries:", header->e_shnum);
		printf("%-50s%x\n","The size of each section header entry:", header->e_shentsize);
		printf("%-50s%x\n","The file offset for program header table:", header->e_phoff);
		printf("%-50s%x\n","Number of program headers:", header->e_phnum);
		printf("%-50s%x\n"NATURAL,"The size of each program header entry:", header->e_phentsize );


		


	}

}

void quit() {
	munmap(map_start, fd_stat.st_size);
	close(fd);
	if (flagDebug) fprintf( stderr, CYAN"%s\n"NATURAL, "Debug: quitting"  );
	exit(0);
}




int main(int argc, char **argv) {
	int choice;

	char input[20];

	while (true) {
				fflush(stdin);

		int i = 0;
		printf("%s\n", "Please choose a function:" );
		while (menu[i].name != NULL && menu[i].fun != NULL) {
			printf("%d) %s\n", i, menu[i].name);
			i++;
		}
						fflush(stdin);

		fgets(input, 20, stdin);
		fflush(stdin);
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
