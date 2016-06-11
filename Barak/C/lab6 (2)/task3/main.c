#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

char dir[1024];
int historyCount = 0;
int variableCount = 0;
typedef struct historyList {
	char* item;
	int myNum;
	struct historyList *next;

} historyList;

typedef struct variables {
	char* item;
	char* meaning;
	struct variables *next;
} variables;


variables *rootVariable;
historyList *history;
void addToHistory(char* toAdd) {
	if (historyCount == 0) {
		history->item = strClone(toAdd);
		history->myNum = 0;
		historyCount++;
		history->next = 0;
	}
	else {
		historyList * pointer = history;
		while (pointer->next != 0) {
			pointer = pointer->next;
		}
		pointer->next = malloc(sizeof(struct historyList));
		pointer->next->item = strClone(toAdd);
		pointer->next->next = 0;
		pointer->next->myNum = historyCount;
		historyCount++;
	}
}

char* getFromHistory(int index) {
	historyList *pointer = history;
	while (index > 0) {
		pointer = pointer->next;
		index--;
	}
	return pointer->item;
}

void printHistory() {
	historyList * pointer = history;
	while (pointer != 0) {
		printf("%d:%s", pointer->myNum, pointer->item);
		pointer = pointer->next;
	}
}







void setVariable(char* var1, char* var2) {
	if (variableCount == 0) {
		rootVariable->item = strClone(var1);
		rootVariable->meaning = strClone(var2);
		rootVariable->next = malloc(sizeof(struct variables));
		variableCount++;
	}
	else {
		int i;
		variables *pointer = rootVariable;
		for (i = 0; i < variableCount; i++) {
			if (pointer->item)
				if (strcmp(pointer->item, var1) == 0) {
					free(pointer->meaning);
					pointer->meaning = strClone(var2);
					return;
				}
			pointer = pointer->next;

		}
		pointer->item = strClone(var1);
		pointer->meaning = strClone(var2);
		pointer->next = malloc(sizeof(struct variables));
		variableCount++;
	}
}


void deleteVariable(char* var) {
	int i;
	variables *pointer = rootVariable;
	variables *prev = rootVariable;
	if (rootVariable && strcmp(pointer->item, var) == 0) {
		/*delet the root*/
		pointer = pointer->next;
		free(rootVariable->item);
		free(rootVariable->meaning);
		free(rootVariable);
		rootVariable = pointer;
		variableCount--;
	}
	else {
		prev = rootVariable;
		pointer = rootVariable->next;
		for (i = 1; i < variableCount; i++) {
			if (strcmp(pointer->item, var) == 0) {
				prev->next = pointer->next;
				free(pointer->item);
				free(pointer->meaning);
				free(pointer);
				variableCount--;
				return;
			}
			pointer = pointer->next;
			prev = prev->next;
		}
		printf("Error: no such variable!\n");
	}
}
void printVariables() {
	int i;
	variables *pointer = rootVariable;
	for (i = 0; i < variableCount; i++) {
		printf("%s=%s\n", pointer->item, pointer->meaning);
		pointer = pointer->next;
	}
}

char* findArgument(char* item) {
	int i;
	variables *pointer = rootVariable;
	for (i = 0; i < variableCount; i++) {
		if (strcmp(pointer->item, item) == 0) {
			return pointer->meaning;
		}
		pointer = pointer->next;
	}
	return "Error";
}

void renameVariable(char* var1, char* var2) {
	int i;
	if (strcmp(findArgument(var2), "Error") == 0) {
		variables *pointer = rootVariable;
		for (i = 0; i < variableCount; i++) {
			if (strcmp(pointer->item, var1) == 0) {
				free(pointer->item);
				pointer->item = strClone(var2);
				return;
			}
			pointer = pointer->next;
		}
		printf("Error: no such variable!\n");
	}
	else {
		printf("Duplicate variable name found...\n");
	}
}
void freeHistory() {
	historyList *current = history;
	historyList *nextItem = NULL;
	while (current) {
		nextItem = current->next;
		free(current->item);
		free(current);
		current = nextItem;
	}
}
void freeVariables() {
	variables *current = rootVariable;
	variables *nextItem = NULL;
	int i;
	for (i = 0; i < variableCount; i++) {
		nextItem = current->next;
		free(current->item);
		free(current->meaning);
		free(current);
		current = nextItem;
	}
	free(current);
}





/*----------------------------------------------------------------*/
int execute(cmdLine *pCmdLine) {
	if (pCmdLine->inputRedirect != 0) {
		/*input redirect:*/
		fclose(stdin);
		stdin = fopen(pCmdLine->inputRedirect, "r");
	}
	if (pCmdLine->outputRedirect != 0) {
		fclose(stdout);
		stdout = fopen(pCmdLine->outputRedirect, "ab+");
	}

	execvp(pCmdLine->arguments[0], pCmdLine->arguments);

	/*error:*/
	perror("Error");
	freeVariables();
	freeHistory();
	freeCmdLines(pCmdLine);
	_exit(0);
	return 0;
}
/*-------------------------------------------------------------*/








void runCommand(cmdLine *line) {

	int duplicate, duplicate2;
	int statusPtr;
	pid_t child1, child2;
	int pipefd[2];
	pipe(pipefd);

	if (line->next) { /*pipe exists*/

		child1 = fork();
		if (child1 < 0) { /*fork has failed!*/
		}

		if (child1 == 0) { /*child process:*/
			fclose(stdout);
			duplicate = dup(pipefd[1]);
			close(pipefd[1]); /*close the write end*/
			close(pipefd[0]);
			execute(line);
			freeCmdLines(line);
		}

		else { /*parent proccess:*/
			close(pipefd[1]);
			child2 = fork();

			if (child2 < 0) {
				/*fork 2 has failed*/
			}

			if (child2 == 0) { /*child2 process*/
				fclose(stdin);
				duplicate2 = dup(pipefd[0]);
				close(pipefd[0]);
				close(pipefd[1]); /*close the write end*/

				line = line->next;
				execute(line);
				freeCmdLines(line);
			}

			else { /*parent process*/
				close(pipefd[0]);
				waitpid (child1, &statusPtr, 0);
				waitpid (child2, &statusPtr, 0);
			}
		}
	}
	else {/*(only one command*/
		child1 = fork();

		if (child1 < 0) { /*fork has failed!*/
		}

		if (child1 == 0) { /*child process:*/
			execute(line);
		}

		else { /*parent proccess:*/
			if (line->blocking)
				waitpid (child1, &statusPtr, 0);
		}
	}
}


int main (int argc , char* argv[], char* envp[]) {
	printf("Helper functions:\n");
	int ** pipesArray = createPipes(4);

	printf("done helper functions:\n");


	history = (struct historyList*) malloc(sizeof(struct historyList));
	rootVariable = (struct variables*) malloc(sizeof(struct variables));
	while (1) { /* main loop:*/
		getcwd(dir, 1024);
		char input[2048] = "";
		printf("%s>", dir);
		fgets(input, 2048, stdin);
		while (input[0] == 10) {
			printf("%s>", dir);
			fgets(input, 2048, stdin);
		}
		if (strncmp(input, "quit", 4) == 0) {
			break;
		}





		/*----------Line exists:----------*/
		cmdLine *line = parseCmdLines(input);
		int i;
		for (i = 0; i < line->argCount; i++) {
			if (line->arguments[i][0] == '$') {
				char *toReplace = findArgument(&line->arguments[i][1]);
				if (strcmp(toReplace, "Error") == 0) {
					printf("Error - No such variable found...\n");
				}
				replaceCmdArg(line, i, toReplace);
			}
		}

		/*get command from history:*/
		if (line->arguments[0][0] == '!') { /*get from history*/
			int num = atoi(&input[1]);
			if (num <= historyCount) {
				addToHistory(getFromHistory(num));
				cmdLine *historyLine = parseCmdLines(getFromHistory(num));
				runCommand(historyLine);
			}
			else {
				printf("Wrong Command!\n");

			}
		}



		else { /*not 'get from history'*/
			if (strcmp(line->arguments[0], "set") == 0) {
				setVariable(line->arguments[1], line->arguments[2]);
			}
			else if (strcmp(line->arguments[0], "rename") == 0) {
				renameVariable(line->arguments[1], line->arguments[2]);
			}
			else if (strcmp(line->arguments[0], "delete") == 0) {
				deleteVariable(line->arguments[1]);
			}
			else if (strcmp(line->arguments[0], "cd") == 0) { /*Is it cd command?*/
				int chdirNum = chdir(line->arguments[1]);

				if (chdirNum != 0) {
					perror("Error: ");
				}

			}

			else { /*not cd, set, rename or delete*/
				runCommand(line);
			}

			addToHistory(input);
			if (strcmp(line->arguments[0], "history") == 0) {
				printHistory();
			}
			else if (strcmp(line->arguments[0], "env") == 0) {
				printVariables();
			}
			/*else if(execute(line) == -1){
			perror("Error");
			freeVariables();
			freeHistory();
			freeCmdLines(line);
			_exit(0);
			     }
			     */
		}
		freeCmdLines(line);
	}
	printf("\nBye!\n");
	if (historyCount > 0)
		freeHistory();
	else
		free(history);

	if (variableCount > 0)
		freeVariables();
	else
		free(rootVariable);
	return 0;
}
