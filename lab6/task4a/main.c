#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define historyLen 10
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
		int* pipefd= malloc(sizeof(int*)*2);
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
	else return pipes[pCmdLine->idx ];
} 





int execute(cmdLine *pCmdLine) {
	pid_t PID = fork();
	fprintf(stderr, "pis id: %d\n", PID );

	//error
	if (PID == -1) {
		perror("ho no! error in fork");
		exit(-1);
	}
	//child
	else if (PID == 0) {
		if(pCmdLine->inputRedirect!=0){
			fclose(stdin);
			stdin= fopen(pCmdLine->inputRedirect,"r");
		}
		if(pCmdLine->outputRedirect!=0){
			fclose(stdout);
			stdout= fopen(pCmdLine->outputRedirect,"w+");
		}

		fprintf(stderr, "executing command: %s\n", pCmdLine->arguments[0] );
		if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
			perror("ho no! error in execv");
			freeCmdLines(pCmdLine);
			exit(-1);
		}
		return 0;
	}
	//parent
	else {
		int status;
		//printf("PID IS: %d\n",PID );
		if (pCmdLine->blocking == 1) waitpid(PID, &status, 0);
	}
	return 0;

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

	printf("%s\n", "test the command: ls | tee | cat | more");

	cmdLine *lineTest = parseCmdLines ("ls | tee | cat | more");
	cmdLine *lineTest2 = lineTest->next;
	cmdLine *lineTest3 = lineTest2->next;
	cmdLine *lineTest4 = lineTest3->next;

	int size;
	cmdLine * pointerLineTest=lineTest;
	while(pointerLineTest){
		size++;
		pointerLineTest=pointerLineTest->next;
	}
	printf("the size of commands: %d\n", size);


	int ** pipes= createPipes(size-1);
	int i;
	for (i=0; i<size-1; i++){
		printf("%d\n",pipes[i][0]);
		printf("%d\n",pipes[i][1]);
	}


	printf("%s:\n",lineTest->arguments[0] );
	if (leftPipe(pipes,lineTest)==NULL)
	{
		printf("left pipe of ls is NULL\n");
	}
	else printf("%s\n",  "error in left pipe of ls");
	
	int* right= rightPipe(pipes,lineTest);
	printf("%d\n",right[0]);
	printf("%d\n",right[1]);
	

	printf("%s:\n",lineTest2->arguments[0] );
	int* left2= leftPipe(pipes,lineTest2);
	int* right2= rightPipe(pipes,lineTest2);
	printf("%d\n",right2[0]);
	printf("%d\n",right2[1]);
	printf("%d\n",left2[0]);
	printf("%d\n",left2[1]);


	printf("%s:\n",lineTest3->arguments[0] );
	int* left3= leftPipe(pipes,lineTest3);
	int* right3= rightPipe(pipes,lineTest3);
	printf("%d\n",right3[0]);
	printf("%d\n",right3[1]);
	printf("%d\n",left3[0]);
	printf("%d\n",left3[1]);


	printf("%s:\n",lineTest4->arguments[0] );
	int* left4= leftPipe(pipes,lineTest4);
	if (rightPipe(pipes,lineTest4)==NULL)
	{
		printf("right pipe of more is NULL\n");
	}
	else printf("%s\n",  "error in right pipe of more");
	printf("%d\n",left4[0]);
	printf("%d\n",left4[1]);
	

	releasePipes(pipes,size-1);
	freeCmdLines(lineTest);
	
	printf("%s\n", "the end of the test");



	printf("%s\n", "lab 6" );
	while (1) {
		char curDir[maxBuffer];
		getcwd (curDir, maxBuffer);
		printf("%s $ ", curDir);
		char readLine [maxBuffer];
		fgets(readLine, maxBuffer, stdin);

		//case: enter
		if (readLine[0] == 10) {
			// printf("%s $ ", curDir );
			// fgets(readLine, maxBuffer, stdin);
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

		if (checkForSpecialArgument(line, readLine)) {
			freeCmdLines(line);
			continue;
		}

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