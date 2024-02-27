#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ssvm.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char* argv[]) {
	// safe call
	// COMMAND CALL - function index
	// COMMAND_REGISTER_FUNCTION - return type & arguments type -> push function index on stack

	FILE* fd = stdin;
	if (argc > 1) {
		FILE* f = fopen(argv[1], "r");
		if (f == NULL) {
			perror("Couldn't open file");
			return -3;
		}
		fd = f;
	}

	// Header start

	const char* protect_field = "PROTECT0";
	char buffer[9] = {0};
	int b = fread(buffer, 8, 1, fd);
	if (b < 1) {
		return -1;
	}

	if (strcmp(buffer, protect_field) != 0) {
		return -2;
	}

	// Switching?
	// BE, LE test

	// Switching?
	// Different operand size (u64/u32)
	// Diffrent floating-point operand size (f64/f32)

	// Stack size in u64
	// Number of std functions
    // std function id
	// Number of internal function
	// offset in CODE section

	// Header end
	// CODE

	struct vm_state vm;
	uint64_t* stack = malloc(4096);
	// custom stack size
	// table for functions

	vm.sp = stack - sizeof(uint64_t);

	while (!feof(fd)) {
		int c = fgetc(fd);
		if (c != EOF) {
			if (c == COMMAND_PUSH) {
				//if (debug) {
				//	printf("push ");
				//}

				uint64_t piece;
				int bytes = fread(&piece, 8, 1, fd);
				if (bytes == 1) {
					//if(debug) printf("%d\n", piece);

					vm.sp += sizeof(uint64_t);
					*vm.sp = piece;
					//vm.sp += sizeof(uint64_t);
				}
			} else if (c == COMMAND_POP) {
				uint64_t* pointer;
				int bytes = fread(&pointer, 8, 1, fd);
				if (bytes == 1) {
					//vm.sp -= sizeof(uint64_t);
					*pointer = *vm.sp;
					vm.sp -= sizeof(uint64_t);
				}
			} else if (c == COMMAND_PRINT) {
				printf("%lx", *vm.sp);
			} else if (c == COMMAND_SEEK_SP) {
				int64_t piece;
				int bytes = fread(&piece, 8, 1, fd);
				if (bytes == 1) {
					vm.sp += piece * sizeof(uint64_t);
				}
			} else if (c == COMMAND_ADD) {
				*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) + *vm.sp; 
				vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_SUB) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) - *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_MUL) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) * *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_DIV) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) / *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_REM) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) % *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_BITWISE_AND) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) & *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_BITWISE_OR) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) | *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_BITWISE_XOR) {
					*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) ^ *vm.sp;
					vm.sp = vm.sp-sizeof(uint64_t);
			} else if (c == COMMAND_CLONE) {
				uint64_t cell = *vm.sp;
				vm.sp += sizeof(uint64_t);
				*vm.sp = cell;
			} else {
				printf("Error! Unknown opcode: 0x%x\n", c);
				free(stack);
				return -4;
			}
		}
	}
	free(stack);
	return 0;
}

