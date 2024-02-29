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
	putchar(COMMAND_PUSH);
	double c = 8;
	fwrite(&c, 8, 1, stdout);
	putchar(COMMAND_ASSERT);

	putchar(COMMAND_OMIT);
	putchar(COMMAND_OMIT);

	putchar(COMMAND_PUSH);
	fwrite(&a, 8, 1, stdout);
	putchar(COMMAND_PRINT);
	putchar(COMMAND_PUSH);
	fwrite(&b, 8, 1, stdout);
	putchar(COMMAND_PRINT);
	putchar(COMMAND_ADD);
	putchar(COMMAND_PUSH);
	int64_t d = 34;
	fwrite(&d, 8, 1, stdout);
	putchar(COMMAND_ASSERT);
}
