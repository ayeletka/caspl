#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define historyLen 30
#define maxBuffer 2048

char history[historyLen][maxBuffer];
int indexH;
typedef struct map
{
	char* name;
	char* value;
	struct map* next;
} map;

map* mapOfVars;

void freeMapOfVars() {
	map* pointer = mapOfVars;
	while (pointer != 0) {
		free(pointer->value);
		free(pointer->name);
		map* temp = pointer->next;
		free(pointer);
		pointer = temp;
	}
}

void delete(char* name) {
	map* pointer = mapOfVars;
	map* prev = mapOfVars;
	while (pointer != 0) {
		if (strcmp(pointer->name, name) == 0) {
			if (pointer == mapOfVars) {
				mapOfVars = mapOfVars->next;
			}
			else {
				prev->next = pointer->next;
			}
			free(pointer->value);
			free(pointer->name);
			free(pointer);
			return;
		}
		prev = pointer;
		pointer = pointer->next;
	}
	fprintf(stderr, "delete error: couldn't find %s\n", name );
}

void set(char* name, char* value) {
	if (mapOfVars == 0) {
		mapOfVars = malloc(sizeof(map));
		mapOfVars->name = strClone(name);
		mapOfVars->value = strClone(value);
		mapOfVars->next = 0;
	}
	else {
		map* pointer = mapOfVars;
		map* prev = mapOfVars;
		while (pointer != 0) {
			if (strcmp(pointer->name, name) == 0) {
				free(pointer->value);
				pointer->value = strClone(value);
				return;
			}
			prev = pointer;
			pointer = pointer->next;
		}

		//pointer is null:
		pointer = malloc(sizeof(map));
		pointer->name = strClone(name);
		pointer->value = strClone(value);
		pointer->next = 0;
		prev->next = pointer;
	}
}

char* find(char* name) {
	map* pointer = mapOfVars;

	while (pointer != 0) {
		if ((strcmp(pointer->name, name) == 0) || pointer->name == name) {
			return (pointer->value);
		}
		pointer = pointer->next;
	}
	fprintf(stderr, "find error: couldn't find %s\n", name);
	return "error";
}

void printVars() {
	map* pointer = mapOfVars;
	printf("----  env variables:  ----\n");
	while (pointer != 0) {
		printf("%s=%s\n", pointer->name, pointer->value);
		pointer = pointer->next;
	}
}

void printHistory() {
	int i;
	for (i = 0; i < indexH; i++)
	{
		printf("history[%d]: %s\n", i, history[i] );
	}
}



//This function receives the number of required pipes and returns an array of pipes.
int **createPipes(int nPipes) {
	int** pipeArray = malloc(sizeof(int*) * nPipes);
	int i;
	for(i=0; i<nPipes; i++){
		int* pipefd= malloc(sizeof(int)*2);
		pipe(pipefd);
		pipeArray[i]=pipefd;
	}
	return pipeArray;
}
//This function receives an array of pipes and an integer indicating the size of the array. The function releases all memory dedicated to the pipes.
void releasePipes(int **pipes, int nPipes){
	int i;
	for(i=0; i<nPipes; i++){
		free(pipes[i]);
	}
	free (pipes);
}

//This function receives an array of pipes and a pointer to a cmdLine structure. 
//It returns the pipe which feeds the process associated with the command.
// That is, the pipe that appears to the left of the process name in the command line.
int *leftPipe(int **pipes, cmdLine *pCmdLine){
	if(pCmdLine->idx == 0) return NULL;
	else return pipes[pCmdLine->idx -1];

}

//This function receives an array of pipes and a pointer to a cmdLine structure.
// It returns the pipe which is the sink of the associated command. 
//That is, the pipe that appears to the right of the process name in the command line.
int *rightPipe(int **pipes, cmdLine *pCmdLine){
if(pCmdLine->next == 0) return NULL;
	else return pipes[pCmdLine->idx];
} 

 void executeOneCommandWithoutFork(cmdLine *commPointer){

		//input redirection
	if(commPointer->inputRedirect){
	fclose(stdin);
	stdin= fopen(commPointer->inputRedirect,"r");
	}
	//output redirection
	if(commPointer->outputRedirect!=0){
		fclose(stdout);
		stdout= fopen(commPointer->outputRedirect,"w+");
	}
	//executing the command
	fprintf(stderr, "executing command: %s\n", commPointer->arguments[0] );


	if (execvp(commPointer->arguments[0], commPointer->arguments) == -1) {
		perror("ho no! error in execv");
		exit(-1);
	}

 }

int executeOneCommand(cmdLine *pCmdLine){


	pid_t PID = fork();
	fprintf(stderr, "pis id: %d\n", PID );

	//error
	if (PID == -1) {
		perror("ho no! error in fork");
		exit(-1);
	}
	//child
	else if (PID == 0) {
		executeOneCommandWithoutFork(pCmdLine);
	}
	//parent
	else {
		int status;
		if (pCmdLine->blocking == 1) waitpid(PID, &status, 0);
	}
	return 0;

}



void execute(cmdLine *pCmdLine) {

	pid_t pid;
	//checking how many commands in the line
	int commCount=0;
	cmdLine * pointerSize=pCmdLine;

	while(pointerSize){
		commCount++;
		pointerSize=pointerSize->next;
	}


	//only one command
	if(commCount==1){
		executeOneCommand(pCmdLine);
	}
	// more than one commands
	else{
		int** pipes= createPipes(commCount-1);
		cmdLine* commPointer = pCmdLine;
		int i;
		for(i=0;i<commCount-1;i++){

			pid = fork();
			fprintf(stderr, "pid of fork number %d: %d\n", i,pid );

			//error
			if (pid == -1) {
				perror("ho no! error in fork");
				exit(-1);
			}
			//child
			else if (pid == 0) {
				if(rightPipe(pipes,commPointer)){
				fprintf(stderr, "(child%d>redirecting stdout to the write end of the pipe…)\n",i);
				fclose(stdout);
				dup(rightPipe(pipes,commPointer)[1]); //dup write end
				close(rightPipe(pipes,commPointer)[1]); 
				}

				if(leftPipe(pipes,commPointer)){
				fprintf(stderr, "(child%d>redirecting stdin to the read end of the pipe…)\n",i);
				fclose(stdin);
				dup(leftPipe(pipes,commPointer)[0]); //dup read end
				close(leftPipe(pipes,commPointer)[0]); 

				}
				if(strcmp(commPointer->arguments[0], "history") == 0){
					printHistory();
				}	
				else executeOneCommandWithoutFork(commPointer);
			}

			//parent
			else{
				int status=0;
				waitpid((pid_t)pid,&status,0);

				if(rightPipe(pipes,commPointer)) 
					close(rightPipe(pipes,commPointer)[1]);
				if(leftPipe(pipes,commPointer)) 
					close(leftPipe(pipes,commPointer)[0]);
					//close(rightPipe(pipes,commPointer)[1]);
											
				commPointer=commPointer->next;

			}
		}

		//last command in the chain
		pid = fork();
		fprintf(stderr, "pid of fork number %d: %d\n", i,pid );

		//error
		if (pid == -1) {
			perror("ho no! error in fork");
			exit(-1);
		}
		//child
		else if (pid == 0) {
			fclose(stdin);
			dup(leftPipe(pipes,commPointer)[0]); //dup read end
			close(leftPipe(pipes,commPointer)[0]);
			executeOneCommandWithoutFork(commPointer);
		}
		//parent
		else{
			int status=0;
			waitpid((pid_t)pid,&status,0);
			close(leftPipe(pipes,commPointer)[0]);
		}

		
		releasePipes(pipes,commCount-1);
	}
}






void printCmdLine(cmdLine *line) {
	int i;
	for (i = 0; i < line->argCount; i++) {
		printf("%s ", line->arguments[i]);
	}
	printf("\n");
}

int replaceDollar(char* toChange) {
	int i;
	for (i = 0; i < (strlen(toChange) - 1); i++) {
		if (toChange[i] == '$') {
			toChange[i] = 0;
			char copy[maxBuffer];
			strcpy(copy, toChange + i + 1);
			char* newString = find(copy);
			if (strcmp(newString, "error") == 0) {
				return -1;
			}
			strcat(toChange, newString);
		}
	}
	return 0;
}

void insertToHistory(char* toInsert) {
	if (toInsert[0] != '!') {
		strcpy(history[indexH++], toInsert);
	}
}

//returns 1 if found special argument, 0 else.
int checkForSpecialArgument(cmdLine *pCmdLine, char* readLine) {
	int foundCommand = 0;
	if (strcmp(pCmdLine->arguments[0], "cd") == 0) {
		if (pCmdLine->arguments[1][0] == '~') {
			chdir(getenv("HOME"));
		}
		else if (chdir(pCmdLine->arguments[1]) < 0) {
			fprintf(stderr, "%s\n", "ho no! error in chdir!" );
		}
		foundCommand = 1;
	}

	else if (strcmp(pCmdLine->arguments[0], "history") == 0) {
		insertToHistory(readLine);
		printHistory();
		return 1;
	}
	else if (strcmp(pCmdLine->arguments[0], "set") == 0) {
		set(pCmdLine->arguments[1], pCmdLine->arguments[2]);
		foundCommand = 1;
	}
	else if ( strcmp(pCmdLine->arguments[0], "delete") == 0) {
		delete(pCmdLine->arguments[1]);
		foundCommand = 1;
	}
	else if (strcmp(pCmdLine->arguments[0], "env") == 0) {
		printVars();
		foundCommand = 1;
	}
	if (foundCommand) {
		insertToHistory(readLine);
		return 1;
	}
	return 0;
}


int main (int argc , char* argv[], char* envp[])
{



	printf("%s\n", "lab 6" );
	while (1) {
		char curDir[maxBuffer];
		getcwd (curDir, maxBuffer);
		printf("%s $ ", curDir);
		char readLine [maxBuffer];
		fgets(readLine, maxBuffer, stdin);

		//case: enter
		if (readLine[0] == 10) {
			continue;
		}

		//case: quit
		if (strncmp(readLine, "quit", 4) == 0) {
			printf("%s\n", "bye" );
			freeMapOfVars();
			exit(0);
		}

		cmdLine *line = parseCmdLines (readLine);
		int i;
		for (i = 0; i < (line->argCount); i++) {
			if (replaceDollar(line->arguments[i]) == -1) {
				strcpy(line->arguments[i], "");
			}
		}

		//case: !n
		if (strncmp(line->arguments[0], "!", 1) == 0) {
			int chosenIndex = atoi(&line->arguments[0][1]);
			if (chosenIndex <= indexH) {
				strcpy(history[indexH], history[chosenIndex]);
				indexH++;
				freeCmdLines(line);
				line = parseCmdLines(history[chosenIndex]);
			}
			else fprintf(stderr, "%s\n", "not inbound" );
		}

		/*if (checkForSpecialArgument(line, readLine)) {
			freeCmdLines(line);
			continue;
		}*/

//case: normal command (including history)
		else {
			insertToHistory(readLine);
			if (strcmp(line->arguments[0], "") != 0) {
				execute(line);
			}
			freeCmdLines(line);
		}
	}
	return 0;

}