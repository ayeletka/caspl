#include "util.h"

#define BUF_SIZE 1024.
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

#define DT_REG 8
#define SEEK_SET 0
#define O_CREATE 64
#define SYS_EXIT 1

extern int system_call();



 typedef struct dirent {
               unsigned long  d_ino;     /* Inode number */
               unsigned long  d_off;     /* Offset to next linux_dirent */
               unsigned short d_len;  /* Length of this linux_dirent */
               char           d_name[8];  /* Filename (null-terminated) */
                                 /* length is actually (d_reclen - 2 -
                                    offsetof(struct linux_dirent, d_name)) */
               
               char           pad;       /* Zero padding byte*/
               char           d_type;    /* File type (only since Linux*/
                                         /* 2.6.4); offset is (d_reclen - 1)*/
               

           }dirent;



void print(char* str){
     system_call(SYS_WRITE, STDOUT, str, strlen(str));    
}
void printchar(char *str){
     system_call(SYS_WRITE, STDOUT, str, 1);    
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
     int returnNum;
     int bytesToRead;
     int j;
     int toDebag=0;
       char* sys_return= "System_Call return num";
      char* sys_id= "System_Call_ID";


for(j=1; j<argc; j++)
     {
      if(strcmp(argv[j],"-d")==0)
      toDebag=1;
  }


     fd = system_call(SYS_OPEN, "/home/ayelet/Desktop/workspace/task4/task2a/DIR", O_RDONLY, 0777);
     if(fd <0){
      print("Error in: opening DIR\n");
      system_call(SYS_EXIT,0,1, 1);
    }
    else{
     debugging(sys_return,itoa(fd),toDebag,3);
     debugging(sys_id,itoa(SYS_OPEN),toDebag,3);
   }
    
     bytesToRead = system_call(SYS_GETDENTS, fd, directoryData, 8192);
     debugging(sys_return,itoa(bytesToRead),toDebag,3);
    debugging(sys_id,itoa(SYS_GETDENTS),toDebag,3);

     if(bytesToRead == -1){
      print("Error in: reads dirctions from DIR to buf\n");
            system_call(SYS_EXIT,0,1, 1);
          }
      dirent *curDir = directoryData;
    

     int i = 0;
     while(i < bytesToRead){
      
  print("Name: ");
  print(curDir-> d_name);
  print("\n");
  print("Length: ");
  print(itoa(curDir->d_len));
    print("\n");

    debugging("Name",curDir-> d_name,toDebag,3);
        debugging("Length",itoa(curDir->d_len),toDebag,3);


  
  i = i + curDir->d_len;
  curDir= directoryData + i;
  
     }
     returnNum= system_call(SYS_CLOSE, fd);
     debugging(sys_return,itoa(returnNum),toDebag,3);
    debugging(sys_id,itoa(SYS_CLOSE),toDebag,3);

     
     print("Done...\n");
     return 0;
}



