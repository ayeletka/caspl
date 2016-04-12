
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {


  	int c=0;
  
	printf("insert a sentence:\n");
	fflush(stdout);

	if (argc==1){
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
	}
	else{
		int argvLength= strlen(argv[1]);
		end:while((c=fgetc(stdin))!='\n'){
			int i;
			for(i=0; i<argvLength;i=i+1){
	  			char a= argv[1][i];
	  			if(c==a)
	  				goto end;
	  			if(a>96&&a<123){
	  				a=a-32;
	  				if(c==a)
	  					goto end;
	  			}

  			}
  			printf("%c", (char) c);
  		
		}

	}
   
 	printf("\n" );

} 
