#include <stdio.h>
#include <math.h>

extern double pow(double x, double y);

int hexvalue(char c){
    if(c<':'&&c>'/')
      c=c-48;
  else if (c<='F'&&c>='A')
    c=c-55;
      return (int)c;
}


//extern int my_func(char* buf);
int main(int argc, char** argv)
{
  printf("value %d \n", hexvalue('B'));
  long long *x_num;
  long long allocated[1];
  x_num = allocated;
  int i;
  for(i = 0; i<25; i++){
    ((char*)x_num)[i] = 0;
  }
  int numOfRounds;
  char str[18];
  for(i = 0; i<25; i++){
    str[i] = 0;
  }

  fflush(stdout);
  printf("%s\n","please insert a num in hexadecimal:" );
  
  fgets(str, 18, stdin);
    printf("strlen(str):%d\n",strlen(str));

  str[strlen(str)-1] = 0;
  printf("Got the number: %s\n", str);
  long long tmp = 0;
  printf("strlen(str):%d\n",strlen(str));
  for(i=0; i<strlen(str); i++){
    printf("str[strlen(str)-i-1]:  %c\n",str[strlen(str)-i-1]);
    tmp = tmp + (pow(16, i))*hexvalue(str[strlen(str)-i-1]);
  }
  *x_num = tmp;

  printf("Got the number: %llX\n", *x_num);
  printf("Number value: %lld\n", *x_num);




  fflush(stdout);
  printf("%s\n","please insert a num of rounds:" );
  scanf("%d", &numOfRounds);
  
  printf("Got the number: %d\n", numOfRounds);

  //calc_func(buf);

  return 0;
}
