#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
char dir[1024];

typedef struct historyList{
  char* item;
  int myNum;
  struct historyList *next;
  
}historyList;

historyList *history;
void addToHistory(char* toAdd){
  if(history->item == 0){
    history->item = toAdd;
    history->myNum = 0;
    history->next = 0;
  }
  else{
    historyList * pointer = history;
    while(pointer->next != 0){
      pointer->myNum++;
      pointer = pointer->next; 
    }
    pointer->next =malloc(sizeof(struct historyList));
    pointer->next->item = toAdd;
    pointer->next->next = 0;
    pointer->next->myNum = 0;
  }
}
void printHistory(){
  printf("History:\n");
  historyList * pointer = history;
  while(pointer != 0){
      printf("%d:%s\n",pointer->myNum, pointer->item);
    pointer = pointer->next;
  }
}


int execute(cmdLine *pCmdLine){
  int* statusPtr;
  int options = 0;
  pid_t child, endId;
  child = fork();
  if(child >= 0){ /*fork has succeeded!*/
    if(child == 0){
      if(strcmp(pCmdLine->arguments[0],"cd")==0){
	int chdirNum = chdir(pCmdLine->arguments[1]);
	if(chdirNum!=0){
	  perror("Error: ");
	}
	return chdirNum;
      }
      else{ /*other command, no 'cd'*/
	return execvp(pCmdLine->arguments[0], pCmdLine->arguments);
      }
    }
    else{/*parent proccess:*/
      if(pCmdLine->blocking==1){
	wait(&statusPtr);
      }
    }
  }
  return 0;
}
int main (int argc , char* argv[], char* envp[])
{
  history = (struct historyList*) malloc(sizeof(struct historyList));
  while(1){
    getcwd(dir, 1024);
    char input[2048] = "";
    printf("\n%s>", dir);
    fgets(input, 2048, stdin);
    while(input[0]==10){
      printf("%s>", dir);
      fgets(input, 2048, stdin);
    }
    int i;
    cmdLine *line = parseCmdLines(input);
    addToHistory(line->arguments[0]);
    if(strcmp(line->arguments[0], "history") == 0){
      printHistory();
    }
    else if(line->arguments[0][0] == '!'){
     int *num = (int*)(input[1]-48);
     printf("%d\n", num);
    }
    else if(execute(line) == -1){
      _exit(1); 
    }
  }
  printf("\nBye!\n");  
  return 0;
}
