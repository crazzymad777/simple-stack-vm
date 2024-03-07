#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ssvm_op.h"
#include "ssvm.h"
#include <string.h>
#include "ssvm_branches.h"
#include "ssvm_matrix.h"
#include <time.h>

int main(int argc, char* argv[]) {
	FILE* fd = stdin;
	if (argc > 1) {
		FILE* f = fopen(argv[1], "r");
		if (f == NULL) {
			perror("Couldn't open file");
			return -3;
		}
		fd = f;
	}

	const char* protect_field = "PROTECT0";
	char buffer[9] = {0};
	int b = fread(buffer, 8, 1, fd);
	if (b < 1) {
		fprintf(stderr, "Invalid header signature");
		return -1;
	}

	if (strcmp(buffer, protect_field) != 0) {
		fprintf(stderr, "Invalid header signature");
		return -2;
	}

	//clock_t begin = clock();
	struct vm_state vm;
	uint64_t* stack = malloc(4096);
	vm.operand_size = sizeof(uint64_t);
	//vm.sp = stack - vm.operand_size;
	*vm.sp = (uint64_t)NULL;
	ssvm_matrix_call(vm, fd, stack);
	free(stack);
	//printf("%d ms\n", clock()-begin);

	if (stdin != fd) {
		fclose(fd);
	}
	return 0;
}
