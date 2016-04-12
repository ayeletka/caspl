#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; /*bss segment*/
int addr6;/*bss segment*/

int foo();/*text segment*/
void point_at(void *p);/*text segment*/

int main (int argc, char** argv){
    int addr2; /*stack segment*/
    int addr3;/*stack segment*/
    char* yos="ree";/*stack segment*/
    int * addr4 = (int*)(malloc(50));/*heap segment*/
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	 point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos); /*stack segment*/
    printf("- addr4: %p\n",addr4); /*heap segment*/
    printf("- &addr4: %p\n",&addr4); /*stack segment*/
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local;/*stack segment*/
	static int addr0 = 2;  /*data segment*/
    static int addr1;/*bss segment*/


    long dist1 = (size_t)&addr6 - (size_t)p;/*dist1 stack segment*/ /*both addr1, p in bss segment*/ /*stack is in higher addresses then text*/
    long dist2 = (size_t)&local - (size_t)p;/*stack segment*/ /* local on stack, p in bss segment, thats why its so big*/ /*stack is in higher addresses then text*/
    long dist3 = (size_t)&foo - (size_t)p;/*stack segment*/ /* foo on text, p in bss segment, they are pretty close to each other*/ /*stack is in higher addresses then text*/
    printf("- &local: %p\n",&local);
    printf("- &p: %p\n",p);
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}
 
