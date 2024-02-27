#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ssvm.h"
#include <stdbool.h>

int main() {
	//bool debug = true;
	// add protect field "PROTECT0"
	// safe call
	// COMMAND CALL - function index
	// COMMAND_REGISTER_FUNCTION - return type & arguments type -> push function index on stack

	struct vm_state vm;
	uint64_t* stack = malloc(4096);
	vm.sp = stack - sizeof(uint64_t);
	FILE* fd = stdin;
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
					vm.sp -= sizeof(uint64_t);
					*pointer = *vm.sp;
					//vm.sp -= sizeof(uint64_t);
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
			}
		}
	}
	free(stack);
}

