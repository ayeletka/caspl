#include<stdio.h>

extern calc_func(long unsigned int *x, int numOfRounds);



int main(int argc , char** argv ){

	long unsigned int x ;
	int numOfRounds ;
	
	
	printf("Please bring long long unsinged number (64bit) number in HEX :");
	scanf("%lx", &x); 		// Reading from the user HEX long int (64bit)
	
	printf("Please bring int number (32bit) number in decimal :");
	scanf("%d", &numOfRounds); 		// Reading from the user DEC  int (32)
	
	calc_func(&x, numOfRounds);
	
	return 0;
}
