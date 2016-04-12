#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int execute(cmdLine *pCmdLine){
  char dir[1024];
  getcwd(dir, 1024);
  
  pid_t pid;
  if((pid = fork()) == -1)
    perror("fork error");
  else if(pid == 0){
    if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1){
      perror("Error received");
      return -1;
    }
  }
      return 0;

}
int main (int argc , char* argv[], char* envp[])
{
  
  printf("Hello!\n");
  char dir[1024];
  getcwd(dir, 1024);
  
  while(1){
    char input[2048] = "";
    printf("\n%s>", dir);
    fgets(input, 2048, stdin);
    while(input[0]==10){
      printf("%s>", dir);
      fgets(input, 2048, stdin);
    }
    cmdLine line = *parseCmdLines(input);
    printf("%s\n", input);
    if(execute(&line) == -1){
     _exit(1); 
    }
  }
  printf("\nBye!\n");
  
  
  return 0;
}