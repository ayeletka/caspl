#include <stdio.h>
#include <stdlib.h>


 
struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

struct virus viruses[10];




unsigned long convertToDecimal(char hex[])
{
    char *hexString;
    int length = 0;
    const int base = 16; // Base of Hexadecimal Number
    unsigned long decimalNumber = 0;
    int i;
    // Find length of Hexadecimal Number
    for (hexString = hex; *hexString != '\0'; hexString++)
    {
        length++;
    }
    // Find Hexadecimal Number
    hexString = hex;
    for (i = 0; *hexString != '\0' && i < length; i++, hexString++)
    {
        // Compare *hexString with ASCII values
        if (*hexString >= 48 && *hexString <= 57)   // is *hexString Between 0-9
        {
            decimalNumber += (((int)(*hexString)) - 48) * pow(base, length - i - 1);
        }
        else if ((*hexString >= 65 && *hexString <= 70))   // is *hexString Between A-F
        {
            decimalNumber += (((int)(*hexString)) - 55) * pow(base, length - i - 1);
        }
        else if (*hexString >= 97 && *hexString <= 102)   // is *hexString Between a-f
        {
            decimalNumber += (((int)(*hexString)) - 87) * pow(base, length - i - 1);
        }
        else
        {
            printf(" Invalid Hexadecimal Number \n");
            return 0;
            
        }
    }
    printf("%lu\n",decimalNumber );
    return decimalNumber;
}

int main(int argc, char **argv) {

	FILE *fp;
	BYTE size=2;
	int i;
	char hexx[]= "56 49 52 55 53"
  convertToDecimal(hexx);
  /* 

   char buffer[100];

   
   fp = fopen("signatures.txt", "w+");

   for (i = 0; i < 11; i++)
   { 
   fseek(fp, SEEK_SET, 0);

   fread(buffer,2*sizeOf(char), 1, fp);
   printf("%s\n", viruses[i]);

	}
   fclose(fp);
*/

    return 0;

 }