#include "stdio.h"
#include "stdlib.h"

void cmd (char *prompt) {
	printf("cmd > %s\n", prompt);
	if (system(prompt)) exit(1);
}

int main (void) {
	// analysis
	cmd(
		"cd analysis && "
		"gcc -o bld bld.c && "
		"./bld"
	);
	cmd("./analysis/main");
	// latex
	cmd("tectonic f4i3.tex -o ../vault/");
	return 0;
}
