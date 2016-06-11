#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int digit_cnt(char* input) {
	int i = 0;
	int count = 0;
	while (input[i] != 0) {
		if (input[i] >= '0' && input[i] <= '9') {
			count++;
			i++;
		}
	}
	return count;
}

int main (int argc , char* argv[], char* envp[]) {
	char* input = argv[1];
	printf("%d\n", digit_cnt(input));
	return 0;
}