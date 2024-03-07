#include <stdio.h>
#include <string.h>

#include "../includes/ssvm.h"

int main() {
	//int64_t a = 32;
	//int64_t b = 2;
	int64_t stack_size = 0;
	printf("PROTECT1");
	fwrite(&stack_size, 8, 1, stdout);

	char* hello = "привет\n";
	size_t siz = strlen(hello) + 1;
	putchar(COMMAND_PUSH);
	fwrite(&siz, 8, 1, stdout);
	putchar(COMMAND_MALLOC);
	putchar(COMMAND_LOAD);
	fwrite(&siz, 8, 1, stdout);
	fwrite(hello, siz, 1, stdout);

	putchar(COMMAND_PRINT_STRING);
}
