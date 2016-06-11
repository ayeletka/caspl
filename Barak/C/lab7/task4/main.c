#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int digit_cnt(char* input){
	int i;
	int count=0;
	for(i = 0; i<strlen(input); i++){
		if(input[i]>='0' && input[i]<='9'){
			count++;
		}
	}
	return count;
}

int main (int argc , char* argv[], char* envp[]) {
	char* input = argv[1];
	printf("%d\n", digit_cnt(input));
	return 0;
}