#include <stdio.h>



//extern int my_func(char* buf);

int main(int argc, char** argv)
{
  unsigned long long  x_num;
  int numOfRounds;
	
  fflush(stdout);
  printf("%s\n","please insert a num in hexadecimal:" );
  fgets(x_num, 64, stdin);
  printf("Got the number: %llx\n", x_num);

 fflush(stdout);
  printf("%s\n","please insert a num of rounds:" );
  fgets(numOfRounds, 32, stdin);
  printf("Got the number: %d\n", numOfRounds);

  //my_func(buf);

  return 0;
}
