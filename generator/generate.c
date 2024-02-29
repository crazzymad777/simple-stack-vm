#include <stdio.h>
#include <string.h>

#include "../ssvm.h"

int main() {
	int64_t a = 32;
	int64_t b = 2;
	printf("PROTECT0");
	//putchar(COMMAND_JUMP);
	//fwrite(&i, 8, 1, stdout);
	putchar(COMMAND_PUSH);
	fwrite(&a, 8, 1, stdout);
	putchar(COMMAND_PUSH);
	fwrite(&b, 8, 1, stdout);
	putchar(COMMAND_RIGHT_SHIFT);
	putchar(COMMAND_PRINT);
	putchar(COMMAND_TO_FP_S);
	// putchar(COMMAND_PRINT_FP);
	putchar(COMMAND_PUSH);
	double c = 8;
	fwrite(&c, 8, 1, stdout);
	putchar(COMMAND_ASSERT);
}
