#include "stdio.h"
#include "stdlib.h"

int cmd (char *prompt) {
	printf("cmd > analysis > %s\n", prompt);
	return system(prompt);
}

int main (void) {
	cmd("gcc -Wall -Wextra -c main.c");
	cmd("gcc -Wall -Wextra -c formater.c");
	cmd("gcc -Wall -Wextra -c analysis.c");
	cmd("gcc -lm -o main main.o formater.o analysis.o");
	return 0;
}
