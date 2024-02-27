#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ssvm.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>

int ssvm_execute(struct vm_state* vm_ptr, FILE* fd, void* stack);
int ssvm_call(struct vm_state vm, FILE* fd, void* stack);

int ssvm_execute(struct vm_state* vm_ptr, FILE* fd, void* stack) {
	struct vm_state vm = *vm_ptr;

	int c = fgetc(fd);
	if (c != EOF) {
		if (c == COMMAND_PUSH) {
			uint64_t piece;
			int bytes = fread(&piece, 8, 1, fd);
			if (bytes == 1) {
				vm.sp += sizeof(uint64_t);
				*vm.sp = piece;
			}
		} else if (c == COMMAND_POP) {
			uint64_t* pointer;
			int bytes = fread(&pointer, 8, 1, fd);
			if (bytes == 1) {
				*pointer = *vm.sp;
				vm.sp -= sizeof(uint64_t);
			}
		} else if (c == COMMAND_PRINT) {
			printf("%lx", *vm.sp);
		} else if (c == COMMAND_PRINT_FP) {
			printf("%lf", *((double*)vm.sp));
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
		} else if (c == COMMAND_OMIT) {
			vm.sp -= sizeof(uint64_t);
		} else if (c == COMMAND_MALLOC) {
			vm.sp = malloc(*vm.sp);
		} else if (c == COMMAND_FREE) {
			free(vm.sp);
			vm.sp = vm.sp-sizeof(uint64_t);
		} else if (c == COMMAND_PRINT_ALL) {
			uint64_t *ptr = vm.sp;
			do {
				printf("0x%x: 0x%x\n", vm.sp-ptr, *ptr);
				ptr -= sizeof(uint64_t);
			} while(ptr != stack);

		} else if (c == COMMAND_FP_ADD) {
			*(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) + *vm.sp_f64;
			vm.sp = vm.sp-sizeof(uint64_t);

		} else if (c == COMMAND_FP_SUB) {
			*(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) - *vm.sp_f64;
			vm.sp = vm.sp-sizeof(uint64_t);

		} else if (c == COMMAND_FP_MUL) {
			*(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) * *vm.sp_f64;
			vm.sp = vm.sp-sizeof(uint64_t);

		} else if (c == COMMAND_FP_DIV) {
			*(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) / *vm.sp_f64;
			vm.sp = vm.sp-sizeof(uint64_t);

		} else if (c == COMMAND_FP_POWER) {
			*(vm.sp_f64-sizeof(uint64_t)) = pow(*(vm.sp_f64-sizeof(uint64_t)), *vm.sp_f64);
			vm.sp = vm.sp-sizeof(uint64_t);

		} else if (c == COMMAND_FP_CEIL) {
			*vm.sp_f64 = ceil(*vm.sp_f64);
		} else if (c == COMMAND_FP_ROUND) {
			*vm.sp_f64 = round(*vm.sp_f64);
		} else if (c == COMMAND_TAKE) {
			*vm.sp = **vm.sp_ptr;
		} else if (c == COMMAND_RIGHT_SHIFT) {
			*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) >> *vm.sp;
		} else if (c == COMMAND_LEFT_SHIFT) {
			*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) << *vm.sp;
		} else if (c == COMMAND_SWAP) {
			uint64_t value = *vm.sp;
			*vm.sp = *(vm.sp-sizeof(uint64_t));
			*(vm.sp-sizeof(uint64_t)) = value;
		} else if (c == COMMAND_JUMP) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (offset == 0) {
					fprintf(stderr, "Simple Stack VM halt!\n");
					return -6;
				}
				fseek(fd, pos + offset, SEEK_SET);
			}
		} else if (c == COMMAND_CALL) {
			*vm_ptr = vm;
			fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			return -5;
		} else if (c == COMMAND_RET) {
			*vm_ptr = vm;
			fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			return -5;
		} else if (c == COMMAND_LOAD || (c >= COMMAND_JUMP_IF_ZERO && c <= COMMAND_JUMP_IF_LESS_OR_EQUAL)) {
			*vm_ptr = vm;
			fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			return -5;
		} else {
			*vm_ptr = vm;
			fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			return -4;
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
		// Debug, default stack size, endiness, operand size, fp operand size, disable FFI, disable STD functions
		// -d - debug
		// -s size - stack size
		// -e {be,le} - endiness
		// -o {32/64} - operand size
		// -f {32/64} - fp operand size
		// -no-ffi - disable ffi
		// -no-std - disable std
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
		fprintf(stderr, "Invalid header signature");
		return -1;
	}

	if (strcmp(buffer, protect_field) != 0) {
		fprintf(stderr, "Invalid header signature");
		return -2;
	}

	// Switching?
	// BE, LE test
	// 0xF0E0D0C0B1A29384

	// Switching?
	// Different operand size (u64/u32)
	// Different floating-point operand size (f64/f32)

	// Stack size in u64
	// Number of std functions
    // std function id
	// Number of internal function
	// offset in CODE section

	// Header end
	// CODE

	struct vm_state vm;
	uint64_t* stack = malloc(4096);
	vm.operand_size = sizeof(uint64_t);
	vm.sp = stack - vm.operand_size;
	ssvm_call(vm, fd, stack);
	free(stack);
	return 0;
}

