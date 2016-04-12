#include <stdio.h>



//extern int my_func(char* buf);

int main(int argc, char** argv)
{
  long long *x_num;
  long long allocated[1];
  x_num = allocated;
  int i;
  for(i = 0; i<16; i++){
    ((char*)x_num)[i] = 0;
  }
  int numOfRounds;
  char str[8];
  for(i = 0; i<8; i++){
    str[i] = 0;
  }

  fflush(stdout);
  printf("%s\n","please insert a num in hexadecimal:" );
  
  fgets(str, 8, stdin);
  printf("Got the number: %s\n", str);
  for(i=0; i<8; i++){
    if(str[i]<':'&&str[i]>'/')
      str[i]=str[i]-48;
    else if (str[i]<='F'&&str[i]>='A') str[i]=str[i]-55;
    ((char*)(x_num))[i]=str[i];
  }

  printf("Got the number: %X\n", *x_num);
  printf("Number value: %lld\n", *x_num);




  fflush(stdout);
  printf("%s\n","please insert a num of rounds:" );
  scanf("%d", &numOfRounds);
  
  printf("Got the number: %d\n", numOfRounds);

  //calc_func(buf);

  return 0;
}
