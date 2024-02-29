#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ssvm_op.h"
#include "ssvm.h"
#include <stdbool.h>
#include <ffi.h>
#include <string.h>

int ssvm_execute(struct vm_state* vm_ptr, FILE* fd, void* stack);
int ssvm_call(struct vm_state vm, FILE* fd, void* stack);

int ssvm_execute(struct vm_state* vm_ptr, FILE* fd, void* stack) {
	struct vm_state vm = *vm_ptr;

	int c = fgetc(fd);
	if (c != EOF) {
		int error_code = 0;
		vm.sp = opcode_matrix[c](vm.sp, fd, &error_code);
		if (error_code != 0) {
			if (error_code == -4) {
				fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			} else if (error_code == -5) {
				fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			}

			*vm_ptr = vm;
			return error_code;
		}
	}
	*vm_ptr = vm;
	return 0;
}

int ssvm_call(struct vm_state vm, FILE* fd, void* stack) {
	while (!feof(fd)) {
		int r = ssvm_execute(&vm, fd, stack);
		if (r != 0) {
			return r;
		}
	}
	return 0;
}

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

	struct vm_state vm;
	uint64_t* stack = malloc(4096);
	vm.operand_size = sizeof(uint64_t);
	vm.sp = stack - vm.operand_size;
	ssvm_call(vm, fd, stack);
	free(stack);

	if (stdin != fd) {
		fclose(fd);
	}
	return 0;
}
