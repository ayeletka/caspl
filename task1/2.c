#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

 
int main(int argc, char **argv)
{
   char c;
   FILE *fp;
   FILE* output=stdout;
   char filter[30];
   bool defaultF=true;
   int argindex;
  
  
   if(argc>1)
   {
      
      for (argindex=1;argindex<argc;argindex++)
      {

      //case 1: just a normal argument
      if (((strcmp(argv[argindex-1],"-i")!=0) && (strcmp(argv[argindex],"-o")!=0) && (strcmp(argv[argindex],"-i")!=0)) || ((argc==2)&& (strcmp(argv[1],"-o")!=0)))

      {
         printf("the first if");
         defaultF=false;
         int argvLength= strlen(argv[argindex]);
         int i;
         strcpy(filter, argv[argindex]);
      }
   
           
      // case 2: file given
      else if(strcmp(argv[argindex],"-i")==0){
         defaultF=false;

         fp = fopen(argv[argindex+1],"r"); 
         if( fp == NULL )
         {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
         }

         char file[30];
         int j=0;
         char f= fgetc(fp);

         while(f!=EOF){
            file[j] = f;
            j=j+1; 
            f= fgetc(fp);         
         }
         printf("the file is %s \n",file);
         int d;
         strcpy(filter, file);  
         fclose(fp);
      }

      //case 3: output to a file
      else if(strcmp(argv[argindex],"-o")==0)
      {
         char fileName[30];
         puts("enter output filename");
            gets(fileName);
            output=fopen(fileName,"w");
            if (output==NULL)
            {
               printf("output file ERROR");
               return 0;
            }
      }
      }
   }

      //this is after the big for
      //case 4: given argument to b filtered
      if(defaultF)
         {
            printf("insert a sentencee:\n");
            fflush(stdout);
           
            while((c=fgetc(stdin))!='\n') {
            switch((char) c) {
               case 'h' :
                  break;
               case 'H' :
                  break;
               default :
                  fputc(c,output);

            }
         
         }
         printf("\n" );
         }
      else{
      printf("insert a sentence:\n");
      fflush(stdout);
      end2:while((c=fgetc(stdin))!='\n'){
            int i;
            for(i=0; i<strlen(filter);i=i+1){
               char a= filter[i];
               if(c==a)
                  goto end2;
               if(a>96&&a<123){
                  a=a-32;
                  if(c==a)
                     goto end2;
               }
            }
            fputc(c,output);
      }
      printf("\n");
      }
   return 0;
}