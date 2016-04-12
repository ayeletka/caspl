#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
bool oneVirus=false;
bool oneVirus2=true;


int memcmp2(const char *cs_in, const char *ct_in, size_t n)
{
  size_t i;  
  const unsigned char * cs = (const unsigned char*) cs_in;
  const unsigned char * ct = (const unsigned char*) ct_in;

  for (i = 0; i < n; i++, cs++, ct++)
  {
    if (*cs < *ct)
    {
      return -1;
    }
    else if (*cs > *ct)
    {
      return 1;
    }
  }
  return 0;
}


void list_print(link* virus_list){
  link* pointer = virus_list;
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

void detect_virus(char *buffer, link *virus_list, unsigned int size){

  link* pointer = virus_list;
  printf("%s\n","welcome to the detect_virus" );
  while(pointer!=0){
    unsigned int length= pointer->virus->length;
    int i=0;
    while(i<size-length+1){
      int comp = memcmp2(pointer->virus->signature, &buffer[i], length);
      if(comp==0 && oneVirus2){
        printf("%s\n","Ho no, you have a virus");
        printf("The starting byte is: %d\n", i);
        printf("The virus name us: %s\n",pointer->virus->name);
        printf("The size of the virus signature: %d\n",length);
        if(oneVirus) oneVirus2=false;

      }
      i++;
    }
    pointer=pointer->next;
  }
  
}

int main(int argc, char **argv) {

	FILE *fp;
  virusList=0;
  FILE *fp2;


  if(argc>2){
    if(strcmp(argv[2],"-f")==0) oneVirus=true;}

    fp2 = fopen(argv[1], "r");
   if( fp2 == NULL )
         {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
         }
    fseek(fp2,0,SEEK_END);
    int fp2Size= ftell(fp2);
    fseek(fp2,-fp2Size,SEEK_CUR);



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
    printf("Printing before:\n");
    list_print(virusList);
    int sizeToDetect= 10000*sizeof(char);
    if(sizeToDetect>fp2Size) sizeToDetect=fp2Size;

    char buffer[sizeToDetect];
    fread(buffer,1,fp2Size,fp2);
    detect_virus(buffer,virusList,sizeToDetect);


    fclose(fp2);
   fclose(fp);
    printf("\n");
  list_free(virusList);

    return 0;

 }