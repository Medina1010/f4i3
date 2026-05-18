#include "stdio.h"
#include "stdlib.h"

int cmd (char *prompt) {
	printf("cmd > analysis > %s\n", prompt);
	return system(prompt);
}

int main (void) {
	cmd("gcc -c main.c");
	cmd("gcc -c formater.c");
	cmd("gcc -lm -o main main.o formater.o");
	return 0;
}
