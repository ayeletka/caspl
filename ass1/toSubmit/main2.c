#include <stdio.h>
#include <stdbool.h>
extern int calc_func(long long *x, int numOfRounds);

bool compare (long long * x, long long * y){

 // printf("x_num is: %016llX", *x);
 // printf("  y_num is: %016llX\n", *y);
	if(*x==*y) return true;
	else return false;

}


int main(int argc, char** argv)
{
  
  unsigned long long x_num= 0L;
  unsigned long long y_num= 0L;
  int numOfRounds=0;
  fflush(stdout);

  //printf("%s\n","Please insert an hex num:");
  scanf("%llX", &x_num);
  //printf("Got the number: %llX\n",x_num);


  //printf("%s\n","Please choose the number of rounds:");
  scanf("%d", &numOfRounds);
  //printf("Got the number: %d\n",numOfRounds);


  calc_func(&x_num, numOfRounds);
  return 0;

}
