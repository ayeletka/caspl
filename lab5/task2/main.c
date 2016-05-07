#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define historyLen 10
#define FREE(X) if(X) free((void*)X)

char* history[historyLen];
int indexH;
int indexVar;

typedef struct map
{
	char* name;
	char* value;
	struct map* next;
}map;

map* mapOfVars;


char* strClone2(const char *source)
{
    char* clone = (char*)malloc(strlen(source) + 1);
    strcpy(clone, source);

    return clone;
}

int replaceCmdArg22(cmdLine *pCmdLine, int num, const char *newString)
{
  if (num >= pCmdLine->argCount)
    return 0;
  printf("newString before clone:%s\n",newString );
  FREE(pCmdLine->arguments[num]);
   printf("pCmdLine->arguments[num] from replaceCmdArg22: %s\n", (pCmdLine->arguments[num]));

  ((char**)pCmdLine->arguments)[num] = strClone2(newString);
  printf("newString from replaceCmdArg22: %s\n", strClone2(newString));
  return 1;
}


void delete(char* name){
	map* pointer= mapOfVars;
	int i;
	for(i=0; i<indexVar;i++){
		if(strcmp(pointer->name,name)==0){
			free(pointer->value);
			free(pointer->name);	
			return;
		}
		pointer=pointer->next;
	}
	fprintf(stderr, "delete error: couldn't find %s\n",name );
}

void set(char* name, char* value){
	if(indexVar==0){
		mapOfVars->name= strClone2(name);
		mapOfVars->value=strClone2(value);
		mapOfVars->next=malloc(sizeof (map));
		indexVar++;
	}
	else{
		map* pointer= mapOfVars;
		int i;
		for(i=0; i<indexVar;i++){
			if(strcmp(pointer->name,name)==0){
				free(pointer->value);
				pointer->value=strClone2(value);
				return;
			}
			pointer=pointer->next;
		}
		pointer->name=strClone2(name);
		pointer->value=strClone2(value);
		pointer->next=malloc(sizeof(map));
		indexVar++;
	}
}

char* find(char* name){
	//printf("%s\n","in find!!!" );
  int i;
  map* pointer= mapOfVars;

	for(i=0; i<indexVar;i++){
		if(strcmp(pointer->name,name)==0){
			return (pointer->value);
		}
		else if(pointer->name==name) return pointer->value;
		printf("error in:%c\n",i );
		pointer=pointer->next;
	}
	fprintf(stderr, "find error: couldn't find %s\n",name );
	return "error";
}


void printVars(){
	int i;
	map* pointer=mapOfVars;
	for(i=0;i<indexVar;i++){
		printf("mapOfVars[%d]: name=%s , value=%s\n",i, pointer->name,pointer->value);
		pointer=pointer->next;
	}
}

void printHistory(){
	int i;
	for (i = 0; i < indexH; i++)
	{
		printf("history[%d]: %s\n",i,history[i] );
	}
}




int execute(cmdLine *pCmdLine){
	
  	pid_t PID= fork();
  	fprintf(stderr, "pis id: %d\n",PID );
  	fprintf(stderr, "executing command: %s\n",pCmdLine->arguments[0] );
  	
  	//error
  	if(PID == -1) {
  		perror("ho no! error in fork");
  		return -1;
  	}
  	//child
  	else if (PID==0){
  		if(strcmp(pCmdLine->arguments[0],"cd")==0){
  			 if(chdir(pCmdLine->arguments[1])<0) {
  			 	perror("ho no! error in chdir!");
  			 	fprintf(stderr, "%s\n","ho no! error in chdir!" );
  			 	return -1;}
  			 else return 0;
  		}
  		else if (strcmp(pCmdLine->arguments[0],"history")==0){
  			printHistory();
  		}
  		else if (strcmp(pCmdLine->arguments[0],"set")==0){
  			set(pCmdLine->arguments[1],pCmdLine->arguments[2]);
  		}
  		else if( strcmp(pCmdLine->arguments[0],"delete")==0){
  			delete(pCmdLine->arguments[1]);
  		}
  		else if (strcmp(pCmdLine->arguments[0],"env")==0){
  			printVars();
  		}
	  else if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1){
	    perror("ho no! error in execv");
	    return -1;
	  }
	  return 0;
	}
	//parent
	else{
		int status;
		//printf("PID IS: %d\n",PID );
		if(pCmdLine->blocking==1) waitpid(PID,&status,0);
	}
	return 0;
	
}



int main (int argc , char* argv[], char* envp[])
{
	printf("%s\n","lab 5" );
	mapOfVars=(struct map*)malloc(sizeof(struct map)); 

	while(1){
		char curDir[2048];
		getcwd (curDir,2048);
		printf("%s $ ", curDir);
		char readLine [2048];
		fgets(readLine,2048,stdin);

 		//case: enter
		while (readLine[0]==10){
			printf("%s $ ",curDir );
			fgets(readLine,2048,stdin);
		}


		//case: quit
		if(strncmp(readLine, "quit", 4)==0){
	      printf("%s\n","bye" );
	      exit(0);
	    }


  		cmdLine *line = parseCmdLines (readLine);
  		int i;
  		int j;

  		for(i=0;i<(line->argCount);i++){
  			//printf("strlen(line->arguments[i]): %d\n", strlen(line->arguments[i]));
  			for(j=0;j<(strlen(line->arguments[i])-1);j++){
  				//printf("arg[i][j]:%c\n",line->arguments[i][j] );
  			if(line->arguments[i][j]=='$'){
  				//printf("%s\n","$$$$$" );
  				line->arguments[i][j]=0;
  				//printf("line->arguments[i]: %s\n",line->arguments[i] );

  				char copy[2048];
  				char* from= line->arguments[i];
  				strcpy(copy,from+j+1);
  				char* newString= find(copy);
  				char* replaced= strcat(line->arguments[i],newString);
  				//printf("replaced: %s\n",replaced );
  				

  				//printf("new string: %s\n",newString );
  				if(strcmp(newString,"error")!=0){
  					//printf("%s\n","-------" );
  				//replaceCmdArg22(line,i,replaced);
  				//printf("the line: %s\n",line->arguments[0] );
  				free (line->arguments[i]);
  				strcpy(readLine,replaced);
  				//printf("readLine: %s\n",readLine );

  				}
  			}
  		}
  	}
  		



	   

	    //case: !n
	    if(strncmp(line->arguments[0],"!",1)==0){
	    	//printf("is !%s\n","" );
	    	//printf("the par is: %s\n",&line->arguments[0][1] );
	    	int chosenIndex= atoi(&line->arguments[0][1]);
	    	if(chosenIndex<=indexH){
	    		//printf("the mum:%d\n",chosenIndex );
	    		char* clone = (char*)malloc(strlen(history[chosenIndex]) + 1);
		    	strcpy(clone, history[chosenIndex]);
				history[indexH]= clone;
		  		indexH++;
		  		//free (clone);
		  		cmdLine line2 = *parseCmdLines(history[chosenIndex]);
		  		if(execute(&line2)==-1) {
		  			fprintf(stderr, "%s\n","error in executing line" );
		  			exit(0);
		  		}
	    	}
	    	else fprintf(stderr, "%s\n","not inbound" );
	    }


	    //case: normal command (including history)
	    else{
	    	char* clone2 = (char*)malloc(strlen(readLine) + 1);
	    	strcpy(clone2, readLine);
			history[indexH]= clone2;
	  		indexH++;
	  		//free (clone2);
	  		cmdLine line = *parseCmdLines (readLine);
	  		
			if(execute (&line)==-1) exit(0);

	    }
		
		



	}
	printf("%s\n","done" );

	return 0;

}