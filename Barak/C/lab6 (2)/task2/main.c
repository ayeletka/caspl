#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

	char dir[1024];
	int duplicate, duplicate2;
	int statusPtr;
	pid_t child1, child2;
	int pipefd[2];

void command(){
	pipe(pipefd);
	child1 = fork();
	if(child1 >= 0){ /*fork has succeeded!*/
		if(child1 == 0){/*child process:*/
		    fclose(stdout);
		    duplicate = dup(pipefd[1]);
		    close(pipefd[1]);
	  		char* args[3];
	    	args[0] = "ls";
			args[1] = "-l";
			args[2] = 0;
			execvp(args[0], args);
			_exit(0);
		}
		else{/*parent proccess:*/
			close(pipefd[1]);
			child2 = fork();
			if(child2>=0){
				if(child2 == 0){/*child2 process*/
					fclose(stdin);
					duplicate2 = dup(pipefd[0]);
					close(pipefd[0]);
			 		char* args[4];
			    	args[0] = "tail";
					args[1] = "-n";
					args[2] = "2";
					args[3] = 0;
					execvp(args[0], args);
					_exit(0);
				}
				else{/*parent process*/
					close(pipefd[0]);
				    waitpid	(child1, &statusPtr, 0);
				    waitpid	(child2, &statusPtr, 0);
				}
			}
		}
	}
}
int main (int argc , char* argv[], char* envp[]){

	printf("Hello!\n");
	command();

	printf("Bye!\n");
	return 0;
}