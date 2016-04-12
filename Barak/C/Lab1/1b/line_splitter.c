
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv) {
  int c;
  while(c != EOF){
      c = fgetc(stdin);
      switch(c){
	case('@'):
	  printf("\n");
	  break;
	
	case('*'):
	
	  
	  break;
	
	  
  }
  printf("%c\n", c);    
  }
  return 0;
}

   
