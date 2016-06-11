#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main (int argc , char* argv[], char* envp[]){
	printf("Hello!\n");
	int statusPtr;
	pid_t child;
	int pipefd[2];
    int pipe(pipefd);

	child = fork();
	if(child >= 0){ /*fork has succeeded!*/
		if(child == 0){
		    printf("Child Process\n");
		    char input[80];
		    int i;
		    fgets(input, 80, stdin);
		    printf("Done receiving in child\n");
		    close(pipefd[0]);
		    write(pipefd[1], input, (strlen(input)+1));
		    _exit(EXIT_SUCCESS);
		}
		else{/*parent proccess:*/
			    printf("parent Process\n");
			    int 	nBytes;
			    char 	readBuffer[80];
			    waitid	(child, &statusPtr, 0);
			    close(pipefd[1]);
			    nBytes = read(pipefd[0], readBuffer, 80);
			    readBuffer[nBytes] = '\0';
			   	printf("Received: %s\n", readBuffer);
			   	close(pipefd[0]);
			    _exit(EXIT_SUCCESS);
		}
	}
	printf("Bye!\n");
	return 0;
}