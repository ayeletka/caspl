#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
int main(int argc, char **argv)
{
   char c;
   FILE *fp;

   if(argc==1)
   {
      printf("insert a sentence:\n");
      fflush(stdout);
     
      while((c=fgetc(stdin))!='\n') {
      switch((char) c) {
         case 'h' :
            break;
         case 'H' :
            break;
         default :
            printf("%c",(char) c);
      }
   
      }
   printf("\n" );
   }
  
   
   // case 1: file
   else if(strcmp(argv[1],"-i")==0){

      fp = fopen(argv[2],"r"); 
      if( fp == NULL )
      {
         perror("Error while opening the file.\n");
         exit(EXIT_FAILURE);
      }

      char* file= malloc(30*sizeof(char));
      int j=0;
      char f= fgetc(fp);

      while(f!=EOF){
         file[j] = f;
         j=j+1; 
         f= fgetc(fp);         
      }
      printf("the file is %s \n",file);

      //the sentence to be filtered
      printf("insert a sentence:\n");
      fflush(stdout);
      
      end:while((c=fgetc(stdin))!='\n'){
            char cf;
            int i;
            for(i=0; i<strlen(file);i=i+1){
               cf = file[i];
               if(c==cf)
                     goto end;
               if(cf>96&&cf<123){
                     cf=cf-32;
                     if(c==cf)
                        goto end;
               }
           }
           printf("%c", (char) c);
      }
      printf("\n" );
 
      fclose(fp);
   }

   else if(argc==2){

      printf("insert a sentence:\n");
      fflush(stdout);
      int argvLength= strlen(argv[1]);
      end2:while((c=fgetc(stdin))!='\n'){
         int i;
         for(i=0; i<argvLength;i=i+1){
            char a= argv[1][i];
            if(c==a)
               goto end2;
            if(a>96&&a<123){
               a=a-32;
               if(c==a)
                  goto end2;
            }

         }
         printf("%c", (char) c);
      }
      printf("\n");
      

   }
   
   return 0;
}