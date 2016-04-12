#include "util.h"
#define SYS_EXIT 1
#define O_RDRW 2
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define SEEK_SET 0


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
     system_call(SYS_WRITE,STDERR, "\n",1);
  
    }
}}

int main (int argc , char* argv[], char* envp[]){
    int inputFile;
int outputFile;
int outputFileIsOpen=0;
int bytesToRead;
/*   int sysReturn1;
  //int sysReturn2;*/
  int filter=0;
int toDebag=0;

  char* sys_exp= "System_Call return num";
   char* sys_id= "System_Call_ID";


    inputFile = STDIN;
     outputFile = STDOUT;


     int i;
     for(i=1; i<argc; i++){
  if(strcmp(argv[i],"-i")==0)
  {
            debugging("The argv",argv[i],toDebag,3);

       inputFile = system_call(SYS_OPEN, argv[i+1], 2 ,777);
        debugging(sys_exp,itoa(inputFile),toDebag,3);
      debugging(sys_id,itoa(SYS_OPEN),toDebag,3);


       if(inputFile<0) {
        system_call(SYS_WRITE,STDOUT,"Invalid input file\n",19);
        system_call(SYS_EXIT,0,1,1);
       }
  }
  else if (strcmp(argv[i],"-o")==0)
  {
                    debugging("The argv",argv[i],toDebag,3);

              char * name=argv[i+1];
              outputFile = system_call(SYS_OPEN, name, 66 ,0777);
              debugging(sys_exp,itoa(outputFile),toDebag,3);
      debugging(sys_id,itoa(SYS_OPEN),toDebag,3);


              if(outputFile<0) {
              system_call(SYS_WRITE,STDOUT,"Invalid output file\n",20);
              system_call(SYS_EXIT,0,1,1);
            }
              outputFileIsOpen=1;
  }

  else if(strcmp(argv[i],"-d")==0) {
        toDebag=1;
        debugging("The argv",argv[i],toDebag,3);
  

  }
}
     
     char c;
     bytesToRead = system_call(SYS_READ, inputFile, &c, 1);
      debugging(sys_exp,itoa(bytesToRead),toDebag,3);
      debugging(sys_id,itoa(SYS_READ),toDebag,3);


    while(bytesToRead>0 && !(c=='\n')) {

      if(!(c=='h')&&!(c=='H')) {
        system_call(SYS_WRITE,outputFile, &c,1);

      }
      else filter=1;
           bytesToRead= system_call(SYS_READ,inputFile,&c,1);
           debugging(sys_exp,itoa(bytesToRead),toDebag,3);
      debugging(sys_id,itoa(SYS_READ),toDebag,3);


     }

     if(filter)
   debugging("is filter","yes",toDebag,3);
 else debugging("is filter","no",toDebag,3);


   


  
    

     if(inputFile!=STDIN){
      int i;
        i=system_call(SYS_CLOSE, inputFile);
        debugging(sys_exp,itoa(i),toDebag,3);
      debugging(sys_id,itoa(SYS_CLOSE),toDebag,3);


        debugging("stdin is",itoa(inputFile),toDebag,3);
     }
     else  debugging("stdin is","stdin",toDebag,3);

     if(outputFileIsOpen){
      int i;
       i=system_call(SYS_CLOSE, outputFile);
        debugging(sys_exp,itoa(i),toDebag,3);
      debugging(sys_id,itoa(SYS_CLOSE),toDebag,3);

              debugging("stdout is",itoa(outputFile),toDebag,3);
     }
     else  debugging("stdout is","STDOUT",toDebag,3);
  system_call(SYS_WRITE,STDOUT, "\n",1);
    system_call(SYS_EXIT,0,1, 1);
    return 0;
}
