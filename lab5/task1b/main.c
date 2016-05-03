#include "LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>




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



	while(1){
		char curDir[2048];
		getcwd (curDir,2048);
		printf("%s $ ", curDir);
		char readLine [2048];
		fgets(readLine,2048,stdin);
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