#include <stdio.h>
 #include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int abs(int n); /* Gets an integer n, and returns the absolute value of n. */
int iprt(int n); /* Prints the value of n followed by a new line, and returns n unchanged */
int cprt(int n); /* Prints the character of ASCII value n followed by a new line, and returns n unchanged.*/
int my_get(int n); /* Ignores n, reads a line from stdin, and returns a number given in that line.  */
int quit(int n);
int plus_one(int n);

struct fun_desc { /*struct*/
  char *name;
  int (*fun)(int);};
struct fun_desc menu[] = { { "plus one", plus_one }, { "abs", abs },{ "Print Integer", iprt},
{ "Print Character", cprt},{ "Get numbers", my_get} , {"Quit", quit} , { NULL, NULL } };

int abs(int n){
	if (n<0){ 
		n=n*-1;
		return n;}
	else return n;}

int iprt(int n){ printf("%d\n", n ); return n;}

int cprt(int n){
	if(n>= 0x20 && n<= 0x7E) printf("%c\n",(char)n);
	else  printf(".\n");
	return n;
}

int my_get(int n){
	int i;
 	char input[50];
 	fflush(stdin);
 	printf("insert a number :\n"); 	
	fgets( input, 50, stdin );
    i = atoi(input);
    return i;
}

int quit(int n){
	exit(n);
}

int plus_one(int n){return n+1;}

int* map(int *array, int arrayLength, int (*f) (int)){
	int* mappedArray = (int*)(malloc(arrayLength*sizeof(int)));
	int i;
	for (i = 0; i < arrayLength; i++)
	{
		mappedArray[i]=f(array[i]);
	}
	return mappedArray;
}

int main(int argc, char **argv){
    int choice;
	int *iarray;
	int len = 4;
	char input[20];

	while(true){
		int i=0;
		printf("%s\n","Please choose a function:" );
		while(menu[i].name!=NULL && menu[i].fun!=NULL){
			printf("%d) %s\n",i,menu[i].name);
    		i++;
		}
		fgets(input,20,stdin);

		choice=atoi(input);

		if(strcmp(input,"")!=0)
		{	

		
		if((0<=choice) && (choice<= (i-1)))
			printf("within bounds\n");
		else
		{
			printf("not within bounds\n");
			quit(0);
		}
		iarray = map(iarray, len, menu[choice].fun);
		}
		else quit(0);
	}
}