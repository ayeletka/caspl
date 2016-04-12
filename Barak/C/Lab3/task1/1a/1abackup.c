#include <stdio.h>
#include <stdlib.h>
  const char* signatures = "signatures.txt";
void printHex(char* buffer, int length){
  int i;
  for(i = 0; i<length; i++){
      printf("%02X ", buffer[i] & 0xff);
    
  }
}

int main(int argc, char **argv) {

 printf("Welcome to the virus detecter!\n");
 unsigned int mySize;
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;
  int signatureSize;
  char* name;
  char signature[4];
  pFile = fopen ("signatures.txt" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  fread(&mySize, 1, 4, pFile);
  
  /*mySize = ntohl(mySize);*/
  printf("Test is: %u\n", mySize);
  char sig[mySize];
  fread(sig, 1, mySize, pFile);

  
  /*02x gets unsigned int!*/
  
  
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  printf("size is: %d\n", lSize);
  rewind (pFile);
  
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  unsigned int word = ntohl((unsigned int*)(buffer+2));
  printf("word is: %02d\n", word);
  
  
    printf("----------------------\n");
     printf("buffer: %s\n", buffer);
     
  rewind(pFile);

	fread(signature, 1, 4, pFile);
      signatureSize = (int)strtol(signature, NULL, 0);
      
     printf("Signature size is: %d\n", signatureSize);

       printHex(signature, 4);

       
     printf("signature is: %d\n", signature);
  printHex(buffer, lSize);
  
  fclose (pFile);
  free (buffer);
  printf("\n");
  
  return 0;
}
