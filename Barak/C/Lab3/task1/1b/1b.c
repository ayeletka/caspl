#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* signatures = "signatures.txt";
typedef struct virus virus;
virus* virusList = 0;
struct virus {
  int length;
  char *signature;
  char *name;
  virus *next;
};
void printHex(char* buffer, int length){
  int i;
  for(i = 0; i<length; i++){
    printf("%02X ", buffer[i] & 0xff);
    
  }
}
void list_print(virus *virusList){
  virus* pointer = virusList;
  printf("\n");
  while(pointer!=0){
        printf("Virus name: %s\n", pointer->name);
	printf("Virus size: %d\n", pointer->length);
	printf("signature:\n");
	printHex(pointer->signature, pointer->length);
	printf("\n\n");
      pointer = pointer->next;
  }
  printf("\n");
}
virus *list_append(virus *virusList, virus *data){
  virus* result;
  if(virusList == 0){
      //Start a new list:
    printf("staring a new list");
    result = data;
  }
  else{
    //add a new virus to the list:
   virus* pointer = virusList;
   while(pointer->next!=0){
     pointer = pointer->next;
   }
   pointer->next = data;
   result = virusList;
  }
  return result;
}
void list_free(virus *virusList){
  if(virusList->next!=0){
      list_free(virusList->next);
  }
  free(virusList->name);
  free(virusList->signature);
  free(virusList);
}


int readNextVirus(FILE* pFile){
  unsigned int mySize;
  unsigned int bytesRead;
  bytesRead = fread(&mySize, 1, 4, pFile);
  if(bytesRead == 0){
    return 0;
  }
  //Read the virus signature:
  char *sig = (char*) malloc(mySize);
  fread(sig, 1, mySize, pFile);
     unsigned int lCurr = ftell(pFile);
    fseek (pFile , 0 , SEEK_END);
    unsigned int lSize = ftell (pFile);
   int reading = lSize - lCurr;
    fseek(pFile, -reading, SEEK_CUR);
  printf("\n");
  //Read the virus name:
  char* tmpBuffer = (char*) malloc(reading);
  fread(tmpBuffer, 1, reading, pFile);

  fseek(pFile, (-reading+1 +strlen(tmpBuffer)), SEEK_CUR);
  printf("\n");
  //create a new virus:
  virus* tmpVirus = malloc(sizeof(virus));
  tmpVirus->length = mySize;
  tmpVirus->signature = sig;
  tmpVirus->name = tmpBuffer;
  tmpVirus->next = 0;
  
  virusList = list_append(virusList, tmpVirus);
  return 1;
}

int main(int argc, char **argv) {
  virusList = 0;
  FILE * pFile;
  pFile = fopen ("signatures.txt" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  printf("\n\n\n-------------------------------------\nWelcome to the virus detecter!\n");
  int reading = 1;
  while(reading != 0){
   unsigned int lCurr = ftell(pFile);
    fseek (pFile , 0 , SEEK_END);
    unsigned int lSize = ftell (pFile);
    reading = lSize - lCurr;
    fseek(pFile, -reading, SEEK_CUR);
    readNextVirus(pFile);
  }
  fclose (pFile);
  printf("\n");
  list_print(virusList);
  list_free(virusList);
  return 0;
}
