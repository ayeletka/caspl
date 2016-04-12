#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct virus virus;

struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

typedef struct link link;
struct link{
        virus* virus;
        link* next; 

        
    };

    

struct link* virusList = 0;
struct virus* viruses[10];


void list_print(link* virus_list){
  link* pointer = virus_list;
  printf("\n");
  while(pointer!=0){
    printf("Virus name: %s\n", pointer->virus->name);
    printf("Virus size: %d\n", pointer->virus->length);
    printf("%s\n", "signature: ");
    int size= pointer->virus->length;
    int j;
    for(j=0;j<size;j++){
      printf("%02x ", pointer->virus->signature[j] & 0xff);
    }
    printf("%s\n\n","" );

      pointer = pointer->next;
  }
  printf("\n");
}

link* list_append(link* virus_list, virus* data){
  if(virus_list == 0){
      /*Start a new list:*/
    printf("staring a new list");
    virus_list = malloc(sizeof (struct link));
    virus_list->virus = data;
    virus_list->next = 0;
  }
  else{
    //add a new virus to the list:
   link* pointer = virus_list;
   while(pointer->next != 0){
     pointer = pointer->next;
   }
   pointer->next = malloc(sizeof(struct link));
   pointer->next->virus = data;
   pointer->next->next = 0;
  }
  return virus_list;
}

void list_free(link *virus_list){
 if(virus_list->next!=0){
      list_free(virus_list->next);
  }
  free(virus_list->virus);
  free(virus_list);
}


int main(int argc, char **argv) {

	FILE *fp;
    virusList=0;

   
   fp = fopen("signatures.txt", "r");
   if( fp == NULL )
         {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
         }

  
    unsigned char length[4] ={0,0,0,0};
    unsigned int bytesRead;
    int reading;
    int i;
   



    for(i=0; i<10;i++)
    {
    unsigned int lCurr = ftell(fp);
    fseek (fp , 0 , SEEK_END);
    unsigned int lSize = ftell (fp);
    reading = lSize - lCurr;
    fseek(fp, -reading, SEEK_CUR);

    bytesRead = fread(length, 1, 2, fp);
    char tmp = length[1];
    length[1] = length[0];
    length[0] = tmp;

    if(bytesRead == 0){printf("%s\n","bad" );}
    int size= *length - 18;
    virus* tmpVirus =  malloc(sizeof(struct virus) + sizeof(char[size]));
    fread(tmpVirus->name, 1, *length-2, fp);
    tmpVirus->length = size;
  
    virusList = list_append(virusList, tmpVirus);

    }


 
   fclose(fp);
    printf("\n");
  list_print(virusList);
  list_free(virusList);

    return 0;

 }