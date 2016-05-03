#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void execute(cmdLine *pCmdLine){
	printf("%s\n", "in execute");
  //printf("Args:%s", pCmdLine->arguments[0]);

  if(execv("/bin/ls", pCmdLine->arguments) == -1){
    perror("Error received");
  }
}


int main (int argc , char* argv[], char* envp[])
{
	printf("%s\n","lab 5" );

	char curDir[2048];
	getcwd (curDir,2048);
	printf("%s\n", curDir);

	while(1){
		char readLine [2048];
		fgets(readLine,2048,stdin);
		printf("The line is: %s\n",readLine );
		cmdLine line = *parseCmdLines (readLine);
 if(strncmp(readLine, "quit", 4)==0){
      printf("%s\n","bye" );
      exit(0);
    }


		execute (&line);



	}
	printf("%s\n","done" );

	return 0;

}