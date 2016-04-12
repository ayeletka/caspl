#include <stdio.h>


int main(int argc, char **argv) {

  int c=0;
  
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
