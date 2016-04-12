#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char **argv){

int iarray[3];
int x=3;
char carray[3];
carray[0] = 't';
iarray[0] = 4;
iarray[1] = 16;

int iarray2[] = {1,2,3};
char carray2[] = {'a','b','c'};
int* iarray2Ptr=iarray2;
char* carray2Ptr=carray2;
char* point;
printf("The sentence contains %d \n" , x);
return 0;
}