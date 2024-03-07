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
	int matrix = 1;

	FILE* fd = stdin;
	if (argc > 1) {
		int s = 0;
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				if (argv[i][1] == 'm') {
					matrix = 1;
				} else if (argv[i][1] == 'b') {
					matrix = 0;
				}
			} else {
				FILE* f = fopen(argv[i], "r");
				if (f == NULL) {
					perror("Couldn't open file");
					return -3;
				}
				fd = f;
			}
		}
	}

	const char* protect_field = "PROTECT1";
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

	size_t stack_size = 0;
	fread(&stack_size, 8, 1, fd);

	if (stack_size == 0) {
		stack_size = 4096;
	}

	struct vm_state vm;
	uint64_t* stack = malloc(stack_size);

	if (stack == NULL) {
		perror("Couldn't allocate stack.");
		return -6;
	}

	vm.operand_size = sizeof(uint64_t);
	vm.sp = stack;
	*vm.sp = (uint64_t)NULL;

	if (matrix == 1) {
		ssvm_matrix_call(vm, fd, stack);
	} else {
		ssvm_branches_call(vm, fd, stack);
	}
	free(stack);

	if (stdin != fd) {
		fclose(fd);
	}
	return 0;
}
