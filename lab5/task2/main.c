#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define historyLen 10

char* history[historyLen];
int indexH;
int indexVar;
typedef struct map
{
	char* name;
	char* value;
	struct map* next;
} map;

map* mapOfVars;

void delete(char* name) {
	map* pointer = mapOfVars;
	int i;
	for (i = 0; i < indexVar; i++) {
		if (strcmp(pointer->name, name) == 0) {
			free(pointer->value);
			free(pointer->name);
			return;
		}
		pointer = pointer->next;
	}
	fprintf(stderr, "delete error: couldn't find %s\n", name );
}

void set(char* name, char* value) {
	if (indexVar == 0) {
		mapOfVars->name = strClone(name);
		mapOfVars->value = strClone(value);
		mapOfVars->next = malloc(sizeof (map));
		indexVar++;
	}
	else {
		map* pointer = mapOfVars;
		int i;
		for (i = 0; i < indexVar; i++) {
			if (strcmp(pointer->name, name) == 0) {
				free(pointer->value);
				pointer->value = strClone(value);
				return;
			}
			pointer = pointer->next;
		}
		pointer->name = strClone(name);
		pointer->value = strClone(value);
		pointer->next = malloc(sizeof(map));
		indexVar++;
	}
}

char* find(char* name) {
	//printf("%s\n","in find!!!" );
	int i;
	map* pointer = mapOfVars;

	for (i = 0; i < indexVar; i++) {
		if (strcmp(pointer->name, name) == 0) {
			return (pointer->value);
		}
		else if (pointer->name == name) return pointer->value;
		printf("error in:%c\n", i );
		pointer = pointer->next;
	}
	fprintf(stderr, "find error: couldn't find %s\n", name );
	return "error";
}


void printVars() {
	int i;
	map* pointer = mapOfVars;
	for (i = 0; i < indexVar; i++) {
		printf("mapOfVars[%d]: name=%s , value=%s\n", i, pointer->name, pointer->value);
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




int execute(cmdLine *pCmdLine) {

	pid_t PID = fork();
	fprintf(stderr, "pis id: %d\n", PID );
	fprintf(stderr, "executing command: %s\n", pCmdLine->arguments[0] );

	//error
	if (PID == -1) {
		perror("ho no! error in fork");
		return -1;
	}
	//child
	else if (PID == 0) {
		if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
			perror("ho no! error in execv");
			return -1;
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

void replaceDollar(char* toChange) {
	int i;
	for (i = 0; i < (strlen(toChange) - 1); i++) {
		if (toChange[i] == '$') {
			toChange[i] = 0;
			char copy[2048];
			strcpy(copy, toChange + i + 1);
			char* newString = find(copy);
			if (strcmp(newString, "error") == 0) {
				fprintf(stderr, "No arg in env to match: %s\n", copy);
				return;
			}
			strcat(toChange, newString);
		}
	}
}

//returns 1 if found special argument, 0 else.
int checkForSpecialArgument(cmdLine *pCmdLine) {
	if (strcmp(pCmdLine->arguments[0], "cd") == 0) {
		if (chdir(pCmdLine->arguments[1]) < 0) {
			fprintf(stderr, "%s\n", "ho no! error in chdir!" );
		}
		return 1;
	}
	if (strcmp(pCmdLine->arguments[0], "history") == 0) {
		printHistory();
		return 1;
	}
	if (strcmp(pCmdLine->arguments[0], "set") == 0) {
		set(pCmdLine->arguments[1], pCmdLine->arguments[2]);
		return 1;
	}
	if ( strcmp(pCmdLine->arguments[0], "delete") == 0) {
		delete(pCmdLine->arguments[1]);
		return 1;
	}
	if (strcmp(pCmdLine->arguments[0], "env") == 0) {
		printVars();
		return 1;
	}
	return 0;
}


int main (int argc , char* argv[], char* envp[])
{
	printf("%s\n", "lab 5" );
	mapOfVars = (struct map*)malloc(sizeof(struct map));
	while (1) {
		char curDir[2048];
		getcwd (curDir, 2048);
		printf("%s $ ", curDir);
		char readLine [2048];
		fgets(readLine, 2048, stdin);

		//case: enter
		if (readLine[0] == 10) {
			// printf("%s $ ", curDir );
			// fgets(readLine, 2048, stdin);
			continue;
		}

		//case: quit
		if (strncmp(readLine, "quit", 4) == 0) {
			printf("%s\n", "bye" );
			exit(0);
		}

		cmdLine *line = parseCmdLines (readLine);
		int i;
		for (i = 0; i < (line->argCount); i++) {
			replaceDollar(line->arguments[i]);
		}

		//case: !n
		if (strncmp(line->arguments[0], "!", 1) == 0) {
			int chosenIndex = atoi(&line->arguments[0][1]);
			if (chosenIndex <= indexH) {
				char* clone = (char*)malloc(strlen(history[chosenIndex]) + 1);
				strcpy(clone, history[chosenIndex]);
				history[indexH] = clone;
				indexH++;
				freeCmdLines(line);
				line = parseCmdLines(history[chosenIndex]);
			}
			else fprintf(stderr, "%s\n", "not inbound" );
		}

		if (checkForSpecialArgument(line)) {
			history[indexH] = strClone(readLine);
			indexH++;
			continue;
		}

//case: normal command (including history)
		else {
			history[indexH] = strClone(readLine);
			indexH++;
			if (execute (line) == -1) exit(0);
		}
	}
	return 0;

}