#include <stdio.h>
 #include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int flagDebug = 0;
char fileName[100];
int unitSize = 2;
void Toggle_Debug_Mode(); 
void Set_File_Name(); 
void Set_Unit_Size(); 
void quit();

struct fun_desc { /*struct*/
  char *name;
  void (*fun)();};
struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Set File Name", Set_File_Name },{ "Set Unit Size", Set_Unit_Size},
{ "Quit", quit} , { NULL, NULL } };

void Toggle_Debug_Mode(){
	if(flagDebug==0){
		flagDebug = 1;
		printf("%s\n","Debug flag now on");
	}
	else{
		flagDebug = 0;
		printf("%s\n",  "Debug flag now off");
	}
}


void Set_Unit_Size(){
	printf("%s\n","Please choose the unit size" );
	char input[20];
	int numberOfUnits;
	fgets(input,20,stdin);
	numberOfUnits=atoi(input);
	if(strcmp(input,"\n")!=0)
	{	
		if((numberOfUnits==1) || (numberOfUnits==2) || (numberOfUnits==2) ){
			unitSize = numberOfUnits;
			if(flagDebug) fprintf(stderr, "Debug: set size to: %d\n",unitSize );
		}
		else
		{
			fprintf(stderr, "%s\n", "Set_Unit_Size : not within bounds");
		}
	}
}

void Set_File_Name(){
 	fflush(stdin);
 	printf("insert a file name :\n"); 	
	fgets( fileName, 100, stdin );
	if(flagDebug) fprintf(stderr, "Debug: file name set to: %s\n",fileName );
}

void quit(){
	if(flagDebug) fprintf(stderr, "%s\n", "Debug: quitting" );
	exit(0);
}




int main(int argc, char **argv){
    int choice;

	char input[20];

	while(true){
		int i=0;
		printf("%s\n","Please choose a function:" );
		while(menu[i].name!=NULL && menu[i].fun!=NULL){
			printf("%d) %s\n",i,menu[i].name);
    		i++;
		}
		fgets(input,20,stdin);
		choice=atoi(input);
			
			if((0<=choice) && (choice<= (i-1)))
				printf("within bounds\n");
			else
			{
				printf("not within bounds\n");
				quit();
			}
			(menu[choice].fun)();
		
		
	}
}