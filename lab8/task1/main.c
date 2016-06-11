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
 char sectionType[40];
void Toggle_Debug_Mode();
void examine_elf_file();
void Set_Unit_Size();
void Print_Section_Names();
void quit();


struct fun_desc { /*struct*/
	char *name;
	void (*fun)();
};
struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Examine elf file", examine_elf_file },{"Print Section Names", Print_Section_Names} , { "Quit", quit}, { NULL, NULL }
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

void switchToString (Elf32_Word type){
	switch(type){
			case 0:
				strcpy(sectionType, "NULL");
				break;
			case 1:
				strcpy(sectionType, "PROGBITS");
				break;
			case 2:
				strcpy(sectionType, "SYMTAB");
				break;
			case 3:
				strcpy(sectionType, "STRTAB");
				break;
			case 4:
				strcpy(sectionType, "RELA");
				break;
			case 5:
				strcpy(sectionType, "HASH");
				break;
			case 6:
				strcpy(sectionType, "DYNAMIC");
				break;
			case 7:
				strcpy(sectionType, "NOTE");
				break;
			case 8:
				strcpy(sectionType, "NOBITS");
				break;
			case 9:
				strcpy(sectionType, "REL");
				break;
			case 10:
				strcpy(sectionType, "SHLIB");
				break;
			case 11:
				strcpy(sectionType, "DYNSYM");
				break;
			case 14:
				strcpy(sectionType, "INIT");
				break;
			case 15:
				strcpy(sectionType, "FINI");
				break;
			case 16:
				strcpy(sectionType, "PREINIT");
				break;
			case 17:
				strcpy(sectionType, "GROUP");
				break;
			case 18:
				strcpy(sectionType, "SHNDX");
				break;
			case 19:
				strcpy(sectionType, "NUM");
				break;
			case 0x60000000:
				strcpy(sectionType, "LOOS");
				break;
			case 0x6ffffff5:
				strcpy(sectionType, "ATTRIBUTES");
				break;
			case 0x6ffffff6:
				strcpy(sectionType, "HASH");
				break;
			case 0x6ffffff7:
				strcpy(sectionType, "LIBLIST");
				break;
			case 0x6ffffff8:
				strcpy(sectionType, "CHECKSUM");
				break;
			case 0x6ffffffa:
				strcpy(sectionType, "LOSUNW");
				break;
			case 0x6ffffffb:
				strcpy(sectionType, "COMDAT");
				break;
			case 0x6ffffffc:
				strcpy(sectionType, "SYMINFO");
				break;
			case 0x6ffffffd:
				strcpy(sectionType, "VERDEF");
				break;
			case 0x6ffffffe:
				strcpy(sectionType, "VERNEED");
				break;
			case 0x6fffffff:
				strcpy(sectionType, "VERSYM");
				break;
			case 0x70000000:
				strcpy(sectionType, "LOPROC");
				break;
			case 0x7fffffff:
				strcpy(sectionType, "HIPROC");
				break;
			case 0x80000000:
				strcpy(sectionType, "LOUSER");
				break;
			case 0x8fffffff:
				strcpy(sectionType, "HIUSER");
				break;
			default:
				printf("\n Type not found\n");
				break;
		}
}

void Print_Section_Names(){
	int numSections = header->e_shnum;
	Elf32_Shdr *sectionHeadersTable;
	int i;

	printf(YELLOW"%s   %-19s   %-20s   %-20s  %-27s  %-15s\n", "index", "SectionName", "SectionAddress", "SectionOffset", "SectionSize", "SectionType");
	for (i = 0; i<numSections; i++) {
		//taking each section with its offset
		sectionHeadersTable = (Elf32_Shdr*)(map_start + header->e_shoff + header->e_shentsize*i);
		//
		Elf32_Shdr* sectionHeaderIndex= map_start + header->e_shoff + (header->e_shstrndx * header->e_shentsize);
		//offset of string table
		char* sectionHeaderStringTable = map_start + sectionHeaderIndex->sh_offset;
		//finding the section name in the string table
		char* sectionName = sectionHeaderStringTable + sectionHeadersTable->sh_name;
		//change (hex) type to a srting
		switchToString(sectionHeadersTable->sh_type);
		printf("[%02d]   %-20s   %-20x   %-20d   %-25d   %-15s\n",
				i, sectionName, sectionHeadersTable->sh_addr, sectionHeadersTable->sh_offset, sectionHeadersTable->sh_size, sectionType);

	}
	printf(NATURAL"%s\n","");

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
		fd = open(fileName, O_RDONLY);
		if (fd <0) {
			fprintf(stderr, "%s didn't open\n",fileName);
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
		printf("%-50s%x\n","The Offset for Section header table:", header->e_shoff);
		printf("%-50s%x\n","Number of entries in the section header table:", header->e_shnum);
		printf("%-50s%x\n","The size of each section header entry:", header->e_shentsize);
		printf("%-50s%x\n","The offset for program header table:", header->e_phoff);
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
