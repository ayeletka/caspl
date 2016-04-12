#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

char dir[1024];
int historyCount = 0;
typedef struct historyList{
  char* item;
  int myNum;
  struct historyList *next;
  
}historyList;

historyList *history;
void addToHistory(char* toAdd){
  if(historyCount == 0){
    history->item=strClone(toAdd);
    history->myNum = 0;
    historyCount++;
    history->next = 0;
  }
  else{
    historyList * pointer = history;
    while(pointer->next != 0){
      pointer = pointer->next; 
    }
    pointer->next =malloc(sizeof(struct historyList));
    pointer->next->item = strClone(toAdd);
    pointer->next->next = 0;
    pointer->next->myNum = historyCount;
    historyCount++;
  }	
}

char* getFromHistory(int index){
  historyList *pointer = history;
  while(index>0){
    pointer = pointer->next;
    index--;
  }
  return pointer->item;
}

void freeHistory(){
  
  historyList *current = history;
  historyList *nextItem = NULL;
  while(current){
    printf("Item: %s", current->item);
    nextItem = current->next;
    free(current->item);
    free(current);
    current = nextItem;
  }
}
void printHistory(){
  historyList * pointer = history;
  while(pointer != 0){
    printf("%d:%s",pointer->myNum, pointer->item);
    pointer = pointer->next;
  }
}


int execute(cmdLine *pCmdLine){
  int statusPtr;
  pid_t child;
  child = fork();
  if(child >= 0){ /*fork has succeeded!*/
    if(child == 0){/*Child proccess*/
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
	waitpid(child, &statusPtr, 0);
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
    printf("%s>", dir);
    fgets(input, 2048, stdin);
    while(input[0]==10){
      printf("%s>", dir);
      fgets(input, 2048, stdin);
    }
    if(strncmp(input, "quit", 4)==0){
      break; 
    }
    cmdLine *line = parseCmdLines(input);
    if(line->arguments[0][0] == '!'){
      int num = atoi(&input[1]);
      if(num <= historyCount){
	addToHistory(getFromHistory(num));
	cmdLine *historyLine = parseCmdLines(getFromHistory(num));
	if(execute(historyLine) == -1){
	  perror("Error");
	}
	freeCmdLines(historyLine);
      }
      else{
	printf("Wrong Command!\n");
      }
    }
    else{
      addToHistory(input);
      if(strcmp(line->arguments[0], "history") == 0){
	printHistory();
      }
      else if(execute(line) == -1){
	perror("Error: ");
	freeHistory();
	freeCmdLines(line);
	_exit(0);
      }
    }
    freeCmdLines(line);
  }
  printf("\nBye!\n");  
  freeHistory();
  return 0;
}
