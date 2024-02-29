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
	putchar(COMMAND_TO_FP_S);
	putchar(COMMAND_PRINT_FP);

	putchar(COMMAND_PUSH);
	int64_t index = 4;
	fwrite(&index, 8, 1, stdout);
	putchar(COMMAND_LOAD_NATIVE_FN);

	int64_t siz = 7;
	putchar(COMMAND_PUSH);
	fwrite(&siz, 8, 1, stdout);
	putchar(COMMAND_MALLOC);
	//putchar(COMMAND_LOAD_NATIVE_FN);

	//putchar(COMMAND_PRINT_ALL);
	char buffer[] = "hello!";
	// int64_t siz = strlen(buffer);
	putchar(COMMAND_LOAD);
	fwrite(&siz, 8, 1, stdout);
	fwrite(buffer, siz, 1, stdout);



	int64_t c = 8;
	putchar(COMMAND_PUSH);
	fwrite(&c, 8, 1, stdout);

	putchar(COMMAND_PRINT_ALL);

//	putchar(COMMAND_CALL_C);
	/*char buffer[] = "hello!";
	int64_t siz = strlen(buffer);

	//putchar(COMMAND_PRINT_ALL);
	putchar(COMMAND_PUSH);
	int64_t index = 4;
	fwrite(&index, 8, 1, stdout);

	putchar(COMMAND_LOAD_NATIVE_FN);

	putchar(COMMAND_PUSH);
	fwrite(&siz, 8, 1, stdout);
	putchar(COMMAND_MALLOC);

	putchar(COMMAND_LOAD);
	fwrite(&siz, 8, 1, stdout);
	fwrite(buffer, siz, 1, stdout);

	//putchar(COMMAND_FREE);

	int64_t c = 8;
	putchar(COMMAND_PUSH);
	fwrite(&c, 8, 1, stdout);

	//putchar(COMMAND_CALL_C);
	putchar(COMMAND_PRINT_ALL);*/
	//putchar(COMMAND_PRINT);
	// putchar(COMMAND_PRINT);
}
