#include "util.h"

#define STDOUT 1
#define STDIN 0
#define STDERR 2

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19

extern int system_call();



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
     system_call(SYS_WRITE,STDERR, "\n",1)''
  
    }
}}




int main (int argc , char* argv[], char* envp[])
{
  int toDebag=0;
  int sysReturn;
    int sysReturn1;
  int sysReturn2;
  int filter=0;


  char* sys_exp= "System_Call return num";
   char* sys_id= "System_Call_ID";

  int input= STDIN;

  int j;
     for(j=1; j<argc; j++)
     {
      if(strcmp(argv[j],"-d")==0) {
        toDebag=1;
        debugging("The argv",argv[j],toDebag,3);
      }


    }

        debugging("The filter characters are","h , H",toDebag,3);

  char * m= "please insert a sentence!\n";
  sysReturn= system_call(SYS_WRITE,STDOUT, m,strlen(m));
  debugging(sys_exp,itoa(sysReturn),toDebag,3);
      debugging(sys_id,itoa(SYS_WRITE),toDebag,3);



char c;
int bytesToRead=0;
   bytesToRead= system_call(SYS_READ,input,&c,1);
  debugging(sys_exp,itoa(bytesToRead),toDebag,3);
        debugging(sys_id,itoa(SYS_READ),toDebag,3);

    debugging("The input is","",toDebag,3);
        debugging("sdtin",&c,toDebag,STDIN);

   if(!(c=='h')&&!(c=='H')) {
    sysReturn1=system_call(SYS_WRITE,STDOUT, &c,1);

  }
  else  filter=1;



  while(bytesToRead>0) {
   bytesToRead= system_call(SYS_READ,STDIN,&c,1);
     debugging("sdtin",&c,toDebag,STDIN);

    if(!(c=='h')&&!(c=='H')) {
      sysReturn2= system_call(SYS_WRITE,STDOUT, &c,1);
    }
    else filter=1;
    
   }
   if(filter)
   debugging("is filter","yes",toDebag,3);
 else debugging("is filter","no",toDebag,3);


         debugging(sys_exp,itoa(sysReturn1),toDebag,3);
                 debugging(sys_id,itoa(SYS_WRITE),toDebag,3);

      debugging(sys_exp,itoa(sysReturn2),toDebag,3);
              debugging(sys_id,itoa(SYS_WRITE),toDebag,3);


  
  sysReturn=system_call(SYS_WRITE,STDOUT, "done!\n",6);
  debugging(sys_exp,itoa(sysReturn),toDebag,3);
  debugging(sys_id,itoa(SYS_WRITE),toDebag,3);


  system_call(SYS_EXIT,0,1, 1);

    return 0;

}
