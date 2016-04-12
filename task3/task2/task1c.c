#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus virus;
struct virus {
    unsigned short length;
    char name[16];
    char signature[];};
typedef struct link link;
struct link{
        virus* virus;
        link* next;};
FILE *fp2;
struct link* virusList = 0;
struct virus* viruses[10];

void cleanVirus(int offset, int length){
  rewind(fp2);
  fseek(fp2, offset, SEEK_CUR);
  char clean[length];
  int i;
  for (i = 0; i< length; i++){
    clean[i] = 0x90;
  }
  fwrite(clean, 1, length, fp2);
  printf("cleaned the virus!!\n");
}

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
    printf("staring a new list");
    virus_list = malloc(sizeof (struct link));
    virus_list->virus = data;
    virus_list->next = 0;
  }
  else{
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
      int comp = memcmp(pointer->virus->signature, &buffer[i], length);
      if(comp==0){
        printf("%s\n","Ho no, you have a virus");
        printf("The starting byte is: %d\n", i);
        printf("The virus name us: %s\n",pointer->virus->name);
        printf("The size of the virus signature: %d\n",length);
        cleanVirus(i, length);
      }
      i++;
    }
    pointer=pointer->next;
  }
}

int main(int argc, char **argv) {

	FILE *fp;
  virusList=0;
  unsigned char length[4] ={0,0,0,0};
  unsigned int bytesRead;
  int reading;
  int i;
  //open the infected file
  fp2 = fopen(argv[1], "rw+");
  if( fp2 == NULL )
         {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
         }
  fseek(fp2,0,SEEK_END);
  int fp2Size= ftell(fp2);
  fseek(fp2,-fp2Size,SEEK_CUR);
  //open the virus file
 fp = fopen("signatures.txt", "r");
 if( fp == NULL )
       {
          perror("Error while opening the file.\n");
          exit(EXIT_FAILURE);
       }
    //adding the 10 viruses to a link
    for(i=0; i<10;i++)
    {
    unsigned int lCurr = ftell(fp);
    fseek (fp , 0 , SEEK_END);
    unsigned int lSize = ftell (fp);
    reading = lSize - lCurr;
    fseek(fp, -reading, SEEK_CUR);
    //littel andian
    bytesRead = fread(length, 1, 2, fp);
    char tmp = length[1];
    length[1] = length[0];
    length[0] = tmp;
    if(bytesRead == 0){exit(EXIT_FAILURE);}
    int size= *length - 18;
    //creating a new virus
    virus* tmpVirus =  malloc(sizeof(struct virus) + sizeof(char[size]));
    fread(tmpVirus->name, 1, *length-2, fp);
    tmpVirus->length = size;
    //creating a virus link, recursivly
    virusList = list_append(virusList, tmpVirus);
    }
    // printing the link of viruses
    list_print(virusList);
    //sending to 'detect' function
    int sizeToDetect= 10000*sizeof(char);
    if(sizeToDetect>fp2Size) sizeToDetect=fp2Size;
    char buffer[sizeToDetect];
    fread(buffer,1,fp2Size,fp2);
    fseek(fp2,-fp2Size,SEEK_CUR);
    detect_virus(buffer,virusList,sizeToDetect);


    fclose(fp2);
    fclose(fp);
    printf("\n");
    list_free(virusList);
    return 0;
 }