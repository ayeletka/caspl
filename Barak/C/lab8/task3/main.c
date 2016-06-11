#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


int numOfStart = 0;
char *addr, *addr2;
int fd1, fd2;
struct stat sb;
size_t length;
int symbolCount = 0;
int duplicate = 0;
char* definedSymbolsArray[10000];
char* missingSymbolsArray[10000];
int missing = 0;

void checkMissing(){
	int i = 0;
	for(i = 0; i< 10000; i++){
		if(missingSymbolsArray[i]!=0){
			printf("no missing symbols: FAILED (%s)\n", missingSymbolsArray[i]);
			missing = 1;
		}
	}
}

void findSymbolDefinitions() {
	int i;
	for (i = 0; i < 10000; i++) {
		if (missingSymbolsArray[i] != 0) {
			int j;
			for (j = 0; j < 10000; j++) {
				if (definedSymbolsArray[j] != 0) {
					if (strcmp(definedSymbolsArray[j], missingSymbolsArray[i]) == 0) {
						missingSymbolsArray[i] = 0;
						break;
					}
				}
			}
		}
	}
}


void printUndefinedSymbols() {
	int i;
	for (i = 0; i < 10000; i++) {
		if (missingSymbolsArray[i] != 0) {
		}
		else
			break;
	}
}

void printSymbolsArray() {
	printf("Sybols are: \n");
	int i;
	for (i = 0; i < 10000; i++) {
		if (definedSymbolsArray[i] != 0) {
			printf("%s\n", definedSymbolsArray[i]);
		}
		else
			break;

	}
}

void checkSymbolExists(char* toCheck) {
	int i;
	for (i = 0; i < 10000; i++) {
		if (definedSymbolsArray[i] == 0)
			break;
		else if (strcmp(definedSymbolsArray[i], toCheck) == 0 ) {
			printf("duplicate check: FAILED (%s)\n", toCheck);
			duplicate = 1;
		}

	}

}
void addUnidentifiedSymbol(char* toAdd) {
	int i;
	if (toAdd == 0)
		return;
	for (i = 0; i < 10000; i++) {
		if (missingSymbolsArray[i] == 0 ) {
			missingSymbolsArray[i] = toAdd;
			break;
		}
	}
}

void addSymbol(char* toAdd) {
	int i;
	if (toAdd == 0)
		return;
	for (i = 0; i < 10000; i++) {
		if (definedSymbolsArray[i] == 0 ) {
			definedSymbolsArray[i] = toAdd;
			break;
		}
	}

}

void parseFile(int fd) {

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ,
	            MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED)
		handle_error("mmap");


	addr2 = mmap(NULL, length, PROT_READ,
	             MAP_PRIVATE, fd, 0);
	if (addr2 == MAP_FAILED)
		handle_error("mmap");

	Elf32_Ehdr *ehdr = (Elf32_Ehdr*)addr;


	int index;
	int numberOfSections = ehdr->e_shnum;
	Elf32_Shdr *shdr;
	Elf32_Sym *sym;
	int  symbolTableCount;
	char * sh_strtab_p;
	char *strtab;

	for (index = 0; index < numberOfSections; index ++) {
		shdr  = (Elf32_Shdr*)(addr + ehdr->e_shoff + ehdr->e_shentsize * index);
		Elf32_Shdr *sh_strtab = (Elf32_Shdr*)(addr + ehdr->e_shoff + (ehdr->e_shstrndx * ehdr->e_shentsize));
		sh_strtab_p = addr + sh_strtab->sh_offset;
		char* name = sh_strtab_p + shdr->sh_name;

		if (strcmp(name, ".symtab") == 0) {
			sym = (Elf32_Sym*) (addr + shdr->sh_offset);
			symbolTableCount = shdr->sh_size / sizeof(Elf32_Sym);
		}
		else if (strcmp(name, ".strtab") == 0) {
			strtab = (addr + shdr->sh_offset);
		}
	}

	int i;

	for (i = 0; i < symbolTableCount; i++) {
		shdr  = (Elf32_Shdr*)(addr + ehdr->e_shoff + ehdr->e_shentsize * sym->st_shndx);


		if ((strtab + sym->st_name) != 0 && strcmp((strtab + sym->st_name), "") != 0) {
			checkSymbolExists((strtab + sym->st_name));
			if (sym->st_shndx == SHN_UNDEF) {
				addUnidentifiedSymbol((strtab + sym->st_name));
			}
			else {
				addSymbol((strtab + sym->st_name));
			}
		}

		if (strcmp((strtab + sym->st_name), "_start") == 0) {
			numOfStart++;
		}

		sym++;
	}

}


int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "%s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
		handle_error("open");
	if (fstat(fd1, &sb) == -1)    /* To obtain file size */
		handle_error("fstat");
	fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1)
		handle_error("open");
	if (fstat(fd2, &sb) == -1)    /* To obtain file size */
		handle_error("fstat");

	parseFile(fd1);
	parseFile(fd2);
	findSymbolDefinitions();
	printUndefinedSymbols();


	if (numOfStart == 0) {
		printf("_start check: FAILED\n");
	}
	else {
		printf("_start check: PASSED\n");
	}
	if (duplicate == 0) {
		printf("duplicate check: PASSED\n");
	}
	checkMissing();

	if (numOfStart == 0 || duplicate != 0||missing!=0){
		exit(-1);
	}
	else
		printf("no missing symbols: PASSED\n");


	exit(EXIT_SUCCESS);
}
