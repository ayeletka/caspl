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

int main(int argc, char *argv[])
{
	char *addr;
	int fd;
	struct stat sb;
	size_t length;

	if (argc != 2) {
		fprintf(stderr, "%s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		handle_error("open");
	if (fstat(fd, &sb) == -1)    /* To obtain file size */
		handle_error("fstat");

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ,
	            MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED)
		handle_error("mmap");

	Elf32_Ehdr *ehdr = (Elf32_Ehdr*)addr;

	printf("Bytes: %.3s\n", addr + 1);

	if (strncmp((char*)ehdr->e_ident, ELFMAG, 3) == 0) {
		printf("Magic number correct!\n");
	}
	else {
		printf("Magic number incorrect... Exiting!\n");
		exit(0);
	}

	printf("%-40s%x\n", "Data encoding:", ehdr->e_ident[EI_DATA]);
	printf("%-40s%x\n", "Entry Point:", ehdr->e_entry);
	printf("%-40s%x\n", "File Offset for Header table:", ehdr->e_shoff);
	printf("%-40s%x\n", "Number of Header entries:", ehdr->e_shnum);
	printf("%-40s%x\n", "The size of each section header:", ehdr->e_shentsize);
	printf("%-40s%x\n", "file offset for program header table:", ehdr->e_phoff);
	printf("%-40s%x\n", "Number of program headers:", ehdr->e_phnum);
	printf("%-40s%x\n", "Program headers size:", ehdr->e_phentsize);

	int index;
	int numberOfSections = ehdr->e_shnum;

	Elf32_Shdr *shdr;


	printf("%s   %-10s   %-20s   %-20s  %-30s\n", " idx", "Name:", "Address", "Offset:", "Size:");
	for (index = 0; index < numberOfSections; index ++) {
		shdr  = (Elf32_Shdr*)(addr + ehdr->e_shoff + ehdr->e_shentsize * index);
		Elf32_Shdr *sh_strtab = (Elf32_Shdr*)(addr + ehdr->e_shoff + (ehdr->e_shstrndx * ehdr->e_shentsize));
		char * sh_strtab_p = addr + sh_strtab->sh_offset;
		char* name = sh_strtab_p + shdr->sh_name;
		printf("[%02d]   %-20s   %-20x   %-8d   %-25d\n", index, name, shdr->sh_addr, shdr->sh_offset, shdr->sh_size);
	}
	printf("\n\n\n");
	printf("%s   %-10s   %-20s   %-20s  %-30s\n", " idx", "Value:", "Section Index", "Section Name:", "Symbol Name:");


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
		char* name;
		shdr  = (Elf32_Shdr*)(addr + ehdr->e_shoff + ehdr->e_shentsize * sym->st_shndx);
		if (sym->st_shndx >= SHN_LORESERVE) {
			name = 0;
		}
		else {
			name = (sh_strtab_p + shdr->sh_name);
		}
		printf("[%02d]   %-20x   %-10d   %-20s   %-10s\n",
		       i,
		       sym->st_value,
		       sym->st_shndx,
		       name,
		       (strtab + sym->st_name)
		      );
		sym++;
	}

	exit(EXIT_SUCCESS);
}
