#include "stdio.h"

int main(void) 
{
   char a[50];

   printf("Please write something: ");
   fgets (a, 50, stdin);
   //scanf("%s", a);
   printf("You entered: %s\n", a);

   return 0;
}