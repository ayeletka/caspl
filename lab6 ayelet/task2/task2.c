#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>




int main (int argc , char* argv[], char* envp[])
{
	int pipefd[2];
	pid_t PID1, PID2;
	

	printf("%s\n", "lab 6" );
	pipe(pipefd);
	PID1 = fork();
	fprintf(stderr, "(fork1: parent_process>created process with id: %d\n", PID1 );

	//error
	if (PID1 == -1) {
		perror("ho no! error in fork");
		exit(-1);
	}
	//child
	else if (PID1 == 0) {
		fprintf(stderr, "%s\n","(child1>redirecting stdout to the write end of the pipe…)" );
		fclose(stdout);
		dup(pipefd[1]); //dup write end
		close(pipefd[1]);
		char* command1[3];
		command1[0]= "ls";
			command1[1]= "-l";
			command1[2]=0;
		fprintf(stderr, "(child1>going to execute cmd: %s)\n",command1[0] );
		if (execvp(command1[0], command1) == -1) {
			perror("ho no! error in execv");
			exit(-1);
		}
	}
	//parent
	else {
		fprintf(stderr, "%s\n","(parent_process>closing the write end of the pipe…)");
		close(pipefd[1]); //close the write end of the pipe
		fprintf(stderr, "%s\n", "(parent_process>forking…)");
		PID2 = fork();
		fprintf(stderr, "(fork2: parent_process>created process with id:%d)\n", PID2);

		if (PID2 == -1) {
		perror("ho no! error in fork");
		exit(-1);
		}
		//child2
		else if (PID2 == 0) {
			fprintf(stderr, "%s\n","(child2>redirecting stdin to the read end of the pipe…)" );
			fclose(stdin);
			dup(pipefd[0]); //dup read-end
			close(pipefd[0]);
			char* command2[4];
			command2[0]= "tail";
			command2[1]= "-n";
			command2[2]="2";
			command2[3]=0;
			fprintf(stderr, "(child2>going to execute cmd:%s)\n",command2[0] );
			if (execvp(command2[0], command2) == -1) {
				perror("ho no! error in execv");
				exit(-1);
			}
		}
		else{
		fprintf(stderr, "%s\n", "(parent_process>closing the read end of the pipe…)");
		close(pipefd[0]);//close the read-end
		int status;
		fprintf(stderr, "%s\n", "(parent_process>waiting for child1 processes to terminate…)");
		waitpid(PID1,&status,0);
		fprintf(stderr, "%s\n", "(parent_process>waiting for child2 processes to terminate…)");
		waitpid(PID2,&status,0);
		printf("%s\n", "(parent_process>exiting…)");
		exit(0);

		}

	}

	return 0;

}