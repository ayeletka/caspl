#include <stdio.h>
#include <stdlib.h>
const char* signatures = "signatures.txt";
void printHex(char* buffer, int length){
  int i;
  for(i = 0; i<length; i++){
    printf("%02X ", buffer[i] & 0xff);
    
  }
}
int readNextVirus(FILE* pFile){
  unsigned int mySize;
  unsigned int bytesRead;
  bytesRead = fread(&mySize, 1, 4, pFile);
  if(bytesRead == 0){
    return 0;
  }

  printf("Size: %d", mySize);
  char sig[mySize];
  fread(sig, 1, mySize, pFile);

  printf("\n");
  unsigned char tmpBuffer[100];
  fread(tmpBuffer, 1, 100, pFile);
  printf("Virus name: %s\n", tmpBuffer);
  printf("Virus size: %d\n", mySize);
  printf("signature is:\n");
  printHex(sig, mySize);
  fseek(pFile, (-99 +strlen(tmpBuffer)), SEEK_CUR);
  printf("\n\n");
  return 1;
}
int main(int argc, char **argv) {
  FILE * pFile;
  pFile = fopen ("signatures.txt" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
  printf("\n\n\n-------------------------------------\nWelcome to the virus detecter!\n");
  int reading;
  while(reading != 0){
   unsigned int lCurr = ftell(pFile);
   printf("%u\n", lCurr);
    fseek (pFile , 0 , SEEK_END);
    unsigned int lSize = ftell (pFile);
    printf("%u\n", lSize);

    reading = lSize - lCurr;
    
    fseek(pFile, -reading, SEEK_CUR);
    printf("To Read: %d\n", reading);
    readNextVirus(pFile);
  }
  fclose (pFile);

  printf("\n");
  
  return 0;
}
