#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



int execute(cmdLine *pCmdLine){
	//printf("%s\n", "in execute");
  	pid_t PID= fork();

  	if(PID == -1) {
  		perror("ho no! error in fork");
  		return -1;
  	}
  	else if (PID==0){
	  if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1){
	    perror("ho no! error in execv");
	    return -1;
	  }
	}
	return 0;
}


int main (int argc , char* argv[], char* envp[])
{
	printf("%s\n","lab 5" );



	while(1){
		char curDir[2048];
		getcwd (curDir,2048);
		printf("\n%s $ ", curDir);
		char readLine [2048]="";
		printf("%s\n","here" );
		fgets(readLine,2048,stdin);
		//printf("%s\n",readLine );
		if(strncmp(readLine, "quit", 4)==0){
	      printf("%s\n","bye" );
	      exit(0);
	    }
		while (readLine[0]==10){
			printf("%s $ ",curDir );
			fgets(readLine,2048,stdin);
		}
		cmdLine line = *parseCmdLines (readLine);
 
		if(execute (&line)==-1) exit(0);



	}
	printf("%s\n","done" );

	return 0;

}