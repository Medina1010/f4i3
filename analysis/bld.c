#include "stdio.h"
#include "stdlib.h"

int cmd (char *prompt) {
	printf("cmd > analysis > %s\n", prompt);
	return system(prompt);
}

int main (void) {
	cmd("gcc -lm -o main main.c");
	return 0;
}
