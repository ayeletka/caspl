#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc , char* argv[], char* envp[])
{
  printf("Hello!\n");
  char dir[1024];
  getcwd(dir, 1024);
  printf("%s", dir);
  char* input = gets(stdin);
  printf("%s\n", input);
    printf("\nBye!\n");

  
  return 0;
}