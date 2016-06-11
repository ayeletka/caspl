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
void *map_start2; // will point to the start of the memory mapped file

void *map_start; // will point to the start of the memory mapped file

int flagDebug = 0;
char fileName[100];
struct stat fd_stat; //keep the size of the file

Elf32_Ehdr *header; // this will point to the header structure
Elf32_Ehdr *header2; // this will point to the header structure

Elf32_Sym* symbolTable;

struct namesList{
    char name[100];
    struct namesList* next;
}namesList;

void addName(char* name){
    printf("Adding name: %s\n", name);
}


int fd;
char sectionType[40];
void Toggle_Debug_Mode();
void examine_elf_file();
void Set_Unit_Size();
void Print_Section_Names();
void print_Symbols();
void link_To();
void quit();



struct fun_desc { /*struct*/
    char *name;
    void (*fun)();
};
struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Examine elf file", examine_elf_file },{"Print Section Names", Print_Section_Names} ,{"Print Symbols" , print_Symbols}, { "Link to", link_To}, { "Quit", quit}, { NULL, NULL }
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

void loadSymbolNames(){
    Elf32_Ehdr* localHeader = (Elf32_Ehdr*) map_start2;
    int numSections = localHeader->e_shnum;
    Elf32_Sym* symbolTableOffset;
    Elf32_Sym* dynimacSymbolTableOffset;
    Elf32_Shdr* sectionHeaderIndex;
    Elf32_Shdr* sectionHeadersTable;

    char* sectionHeaderStringTable;
    char* stringTableOffset;
    int symbolTableSize;
    int dynamicSymbolTableSize;
    char* dynamicSymbolsStringTableOffset;

    int dynamicSymbolsStringTableSectionHeaderIndex;

    int i;
    for (i = 0; i<numSections; i++) {

        sectionHeadersTable = (Elf32_Shdr*)(map_start2 + localHeader->e_shoff + localHeader->e_shentsize*i);
        sectionHeaderIndex = (Elf32_Shdr*)map_start2 + localHeader->e_shoff + (localHeader->e_shstrndx * localHeader->e_shentsize);
        printf("Loading header: %d\n", i);


        sectionHeaderStringTable = map_start2 + sectionHeaderIndex->sh_offset;
        printf("Loading headersssss: %d\n", i);

        char* sectionName = sectionHeaderStringTable + sectionHeadersTable->sh_name;
        if(strcmp(sectionName, ".symtab") == 0){
            symbolTableOffset = (Elf32_Sym*) (map_start2 + sectionHeadersTable->sh_offset);
            symbolTableSize = sectionHeadersTable->sh_size;

        }
        if(strcmp(sectionName, ".dynsym") == 0){
            dynimacSymbolTableOffset = (Elf32_Sym*) (map_start2 + sectionHeadersTable->sh_offset);
            dynamicSymbolTableSize = sectionHeadersTable->sh_size;
            dynamicSymbolsStringTableSectionHeaderIndex = sectionHeadersTable->sh_link;
            printf("Index: %d\n", dynamicSymbolsStringTableSectionHeaderIndex);
        }
        if(strcmp(sectionName, ".strtab") == 0){
            stringTableOffset = map_start2 + sectionHeadersTable->sh_offset;
        }

    }

    sectionHeadersTable = (Elf32_Shdr*)(map_start2 + localHeader->e_shoff + localHeader->e_shentsize*dynamicSymbolsStringTableSectionHeaderIndex);
    dynamicSymbolsStringTableOffset = map_start2 + sectionHeadersTable->sh_offset;


    //#######################################################################
    //Get names of dynamic symbols:
    int numOfSymbols = dynamicSymbolTableSize/16;
    for(i = 0; i < numOfSymbols; i++) {
        sectionHeadersTable = (Elf32_Shdr*)(map_start2 + localHeader->e_shoff + localHeader->e_shentsize*dynimacSymbolTableOffset->st_shndx);
        char* name = dynamicSymbolsStringTableOffset + dynimacSymbolTableOffset->st_name;
        addName(name);
        dynimacSymbolTableOffset++;
    }
    printf("\n\n\n");


    //#######################################################################
    //Get names of symbols:
    numOfSymbols = symbolTableSize/16;
    for(i = 0; i < numOfSymbols; i++) {
        sectionHeadersTable = (Elf32_Shdr*)(map_start2 + localHeader->e_shoff + localHeader->e_shentsize*symbolTableOffset->st_shndx);
        char * name = stringTableOffset + symbolTableOffset->st_name;
        addName(name);
        symbolTableOffset++;
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


void print_Symbols(){
    int numSections = header->e_shnum;
    Elf32_Sym* symbolTableOffset;
    Elf32_Sym* dynimacSymbolTableOffset;

    Elf32_Shdr* sectionHeaderIndex;
    Elf32_Shdr* sectionHeadersTable;

    char* sectionHeaderStringTable;
    char* stringTableOffset;
    int symbolTableSize;
    int dynamicSymbolTableSize;
    char* dynamicSymbolsStringTableOffset;

    int dynamicSymbolsStringTableSectionHeaderIndex;

    int i;
    for (i = 0; i<numSections; i++) {

        sectionHeadersTable = (Elf32_Shdr*)(map_start + header->e_shoff + header->e_shentsize*i);
        sectionHeaderIndex= map_start + header->e_shoff + (header->e_shstrndx * header->e_shentsize);
        sectionHeaderStringTable = map_start + sectionHeaderIndex->sh_offset;

        char* sectionName = sectionHeaderStringTable + sectionHeadersTable->sh_name;
        if(strcmp(sectionName, ".symtab") == 0){
            symbolTableOffset = (Elf32_Sym*) (map_start + sectionHeadersTable->sh_offset);
           symbolTableSize = sectionHeadersTable->sh_size;

        }

        if(strcmp(sectionName, ".dynsym") == 0){
            dynimacSymbolTableOffset = (Elf32_Sym*) (map_start + sectionHeadersTable->sh_offset);
            dynamicSymbolTableSize = sectionHeadersTable->sh_size;
            dynamicSymbolsStringTableSectionHeaderIndex = sectionHeadersTable->sh_link;
            printf("Index: %d\n", dynamicSymbolsStringTableSectionHeaderIndex);
        }
        if(strcmp(sectionName, ".strtab") == 0){
            stringTableOffset = map_start + sectionHeadersTable->sh_offset;
        }

    }

    sectionHeadersTable = (Elf32_Shdr*)(map_start + header->e_shoff + header->e_shentsize*dynamicSymbolsStringTableSectionHeaderIndex);
    dynamicSymbolsStringTableOffset = map_start + sectionHeadersTable->sh_offset;
    

    //#######################################################################
    //dynamic symbol table print:
    int numOfSymbols = dynamicSymbolTableSize/16;
    printf("Dynamic symbol table - Number of symbols: %d\n", numOfSymbols);
    printf("%s   %-10s   %-10s   %-10s  %s\n", "Num", "Value", "SectionIndex", "SectionName", "SymbolName");
    for(i = 0; i < numOfSymbols; i++) {
        sectionHeadersTable = (Elf32_Shdr*)(map_start + header->e_shoff + header->e_shentsize*dynimacSymbolTableOffset->st_shndx);
        char * sectionName;
            sectionName = (sectionHeaderStringTable + sectionHeadersTable->sh_name);

        printf("%d:   %-10x   %-10x   %-10s   %-10s\n",
               i, dynimacSymbolTableOffset->st_value, dynimacSymbolTableOffset->st_shndx, sectionName, (dynamicSymbolsStringTableOffset + dynimacSymbolTableOffset->st_name));
        dynimacSymbolTableOffset++;
    }
    printf("\n\n\n");


    //#######################################################################
    //Symbol table print:
    numOfSymbols = symbolTableSize/16;
    printf("Symbol table - Number of symbols: %d\n", numOfSymbols);
    printf("%s   %-10s   %-10s   %-10s  %s\n", "Num", "Value", "SectionIndex", "SectionName", "SymbolName");
    for(i = 0; i < numOfSymbols; i++) {
        sectionHeadersTable = (Elf32_Shdr*)(map_start + header->e_shoff + header->e_shentsize*symbolTableOffset->st_shndx);
        char * sectionName;
        if (symbolTableOffset->st_shndx >= SHN_LORESERVE) {
            sectionName = 0;
        }
        else{
            sectionName = (sectionHeaderStringTable + sectionHeadersTable->sh_name);
        }

        printf("%d:   %-10x   %-10x   %-10s   %-10s\n",
               i, symbolTableOffset->st_value, symbolTableOffset->st_shndx, sectionName, (stringTableOffset + symbolTableOffset->st_name));
        symbolTableOffset++;
    }


    printf(NATURAL"%s\n","");



    // int numOfSymbols;
    // int i;
    //


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
    scanf("\n%s", fileName);
    //fgets( fileName, 100, 0);

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
    fflush(stdin);


}

void readElfFile() {
    printf("insert a file name :\n");
    scanf("\n%s", fileName);
    //fgets( fileName, 100, 0);

    if (flagDebug) fprintf(stderr, CYAN "Debug: file name set to: %s\n"NATURAL, fileName);


    if (fileName == NULL) {
        fprintf(stderr, "%s\n", "file name in null");
        return;
    }
    else {
        fd = open(fileName, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "%s didn't open\n", fileName);
        }
        if (fstat(fd, &fd_stat) != 0) {
            perror("stat failed");
            exit(-1);
        }
        if ((map_start2 = mmap(0, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
            perror("mmap failed");
            munmap(map_start2, fd_stat.st_size);

            exit(-1);
        }

    }
}

void link_To() {
    readElfFile();
    loadSymbolNames();
}

void quit() {
    munmap(map_start, fd_stat.st_size);
    munmap(map_start2, fd_stat.st_size);
    close(fd);
    if (flagDebug) fprintf( stderr, CYAN"%s\n"NATURAL, "Debug: quitting"  );
    exit(0);
}




int main(int argc, char **argv) {
    int choice = 0;

    // char input[20];

    while (true) {


        int i = 0;
        printf("%s\n", "Please choose a function:" );
        while (menu[i].name != NULL && menu[i].fun != NULL) {
            printf("%d) %s\n", i, menu[i].name);
            i++;
        }

        scanf("%d", &choice);

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
