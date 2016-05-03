#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void execute(cmdLine *pCmdLine){
  printf("Args:%s", pCmdLine->arguments[0]);
    char dir[1024];
  getcwd(dir, 1024);
  if(execv("/bin/ls", pCmdLine->arguments) == -1){
    perror("Error received");
  }
}
int main (int argc , char* argv[], char* envp[])
{
  
  printf("Hello!\n");
  char dir[1024];
  getcwd(dir, 1024);
  
  while(1){
    char input[2048];
    printf("\n%s>", dir);
    fgets(input, 2048, stdin);
    cmdLine line = *parseCmdLines(input);
    printf("%s\n", input);
    if(strcmp(input,"quit")==0){
      printf("%s\n","bye" );
      exit(0);
    }
    execute(&line);
  }
  printf("\nBye!\n");
  
  
  return 0;
}