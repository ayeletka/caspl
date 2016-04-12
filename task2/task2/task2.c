#include <stdio.h>
 #include <stdlib.h>


int quit(int n){
	 exit(n);
}
int plus_one(int n) {
  return n+1;
}
int abs(int n)/* Gets an integer n, and returns the absolute value of n. */
{
	int i= (-2);
	if(n<0) return(n+ (i*n));
	else return n;
}
int iprt(int n) /* Prints the value of n followed by a new line, and returns n unchanged */
{
	printf("%d\n", n );
	return n;
}
int cprt(int n) /* Prints the character of ASCII value n followed by a new line, and returns n unchanged. 
                 If n is not between 0x20 and 0x7E, print the dot ('.') character instead. */
{
	char c= n;
	if (33<n && n< 126)
		printf("%c\n",c );
	else
		printf(".\n");
	return n;

}
int my_get(int n) /* Ignores n, reads a line from stdin, and returns a number given in that line.  */
 {
 	int i;
 	 printf("insert a  number :\n");
 	 char input[128];
 	 gets(input);
     i = atoi(input);
     return i;

 }


int* map(int *array, int arrayLength, int (*f) (int)){
	int* mappedArray = (int*)(malloc(arrayLength*sizeof(int)));
	int i;
	for (i = 0; i < arrayLength; i++)
		{
		 mappedArray[i]= f(array[i]);
		}
	return mappedArray;
	}
	 
int main(int argc, char **argv){
	int len = 4;
	int arr1[len];
	int* arr2 = map(arr1, len, my_get);
	int* arr3 = map(arr2, len, iprt);
	int* arr4 = map(arr3, len, abs);
	int* arr5 = map(arr4, len, plus_one);
	int* arr6 = map(arr5, len, cprt);
	free(arr2);
	free(arr3);
	free(arr4);
	free(arr5);
	free(arr6);
}
