#include <stdio.h>
#include <stdlib.h>

void printHex(char* buffer, int length){
  int i;
  for(i = 0; i<length; i++){
      printf("%02X ", buffer[i] & 0xff);
    
  }
}

int main(int argc, char **argv) {
  

  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( "exampleFile.sdx" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  printf("size is: %d\n", lSize);
  rewind (pFile);

  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

     printf("buffer: %s\n", buffer);
     
  printHex(buffer, lSize);
  fclose (pFile);
  free (buffer);
  printf("\n");
  return 0;
}
