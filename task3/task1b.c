#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

struct virus* viruses[10];



int main(int argc, char **argv) {

	FILE *fp;

   
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
    printf("To Read: %d\n", reading);
    printf("lCurr: %u\n",lCurr );
    bytesRead = fread(length, 1, 2, fp);
    char tmp = length[1];
    length[1] = length[0];
    length[0] = tmp;

    if(bytesRead == 0){printf("%s\n","bad" );}
    int size= *length - 18;
    viruses[i] =  malloc(sizeof(struct virus) + sizeof(char[size]));
    viruses[i]->length=(unsigned short) size;

    fread(viruses[i]->name, 1, *length-2, fp);
    printf("virus name: %s\n",viruses[i]->name );
    printf("virus length: %d\n", size);

    int j;
    printf("%s\n", "signature: ");
    for(j=0;j<size;j++){
      printf("%02x ", viruses[i]->signature[j] & 0xff);
    }
    printf("%s\n\n","" );
    }


 
   fclose(fp);


    return 0;

 }