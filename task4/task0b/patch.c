#include "util.h"

#define STDOUT 1

#define SYS_EXIT 1
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19


int main (int argc , char* argv[], char* envp[])
{

  char * msg= "hello you!\n";
  system_call(SYS_WRITE,STDOUT, msg,strlen(msg));

  if(argc!=3){
   system_call(SYS_WRITE,STDOUT,"Wrong parameters...\n", 20);
  system_call(SYS_EXIT,0,1, 1);

  
}

char* fileName = argv[1];
  char* name = argv[2];
  int fileInt;

  fileInt= system_call(SYS_OPEN,fileName,2,777);
  system_call(SYS_LSEEK,fileInt,657,0);
  system_call(SYS_WRITE,fileInt,name,strlen(name));


  system_call(SYS_CLOSE, fileInt);
  system_call(SYS_WRITE,STDOUT, "done!\n",6);
    system_call(SYS_EXIT,0,1, 1);



}
