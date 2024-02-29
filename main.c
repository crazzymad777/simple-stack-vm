#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ssvm_op.h"
#include "ssvm.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <dlfcn.h>
#include <ffi.h>

int ssvm_execute(struct vm_state* vm_ptr, FILE* fd, void* stack);
int ssvm_call(struct vm_state vm, FILE* fd, void* stack);

int ssvm_execute(struct vm_state* vm_ptr, FILE* fd, void* stack) {
	struct vm_state vm = *vm_ptr;

	int c = fgetc(fd);
	if (c != EOF) {
		if (c == COMMAND_PUSH) {
			int error_code = 0;
			vm.sp = op_push(vm.sp, fd, &error_code);
		} else if (c == COMMAND_POP) {
			// move?
			// **sp = *(sp-1)
			// sp -= 2

			int error_code = 0;
			vm.sp = op_pop(vm.sp, fd, &error_code);
			if (error_code != 0) {
				*vm_ptr = vm;
				return error_code;
			}
		} else if (c == COMMAND_PRINT) {
			int error_code = 0;
			vm.sp = op_print(vm.sp, fd, &error_code);
		} else if (c == COMMAND_PRINT_FP) {
			int error_code = 0;
			vm.sp = op_print_fp(vm.sp, fd, &error_code);
		} else if (c == COMMAND_SEEK_SP) {
			int error_code = 0;
			vm.sp = op_seek_sp(vm.sp, fd, &error_code);
		} else if (c == COMMAND_ADD) {
			int error_code = 0;
			vm.sp = op_add(vm.sp, fd, &error_code);
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
			*vm.sp = (uint64_t)malloc(*vm.sp);
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
			vm.sp = vm.sp-sizeof(uint64_t);
		} else if (c == COMMAND_LEFT_SHIFT) {
			*(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) << *vm.sp;
			vm.sp = vm.sp-sizeof(uint64_t);
		} else if (c == COMMAND_SWAP) {
			uint64_t value = *vm.sp;
			*vm.sp = *(vm.sp-sizeof(uint64_t));
			*(vm.sp-sizeof(uint64_t)) = value;
		} else if (c == COMMAND_TO_FP) {
			*vm.sp_f64 = *vm.sp;
		} else if (c == COMMAND_TO_FP_S) {
			*vm.sp_f64 = *vm.sp_s;
		} else if (c == COMMAND_TO_INTEGER) {
			*vm.sp = *vm.sp_f64;
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
		} else if (c == COMMAND_CALL_C) {
			/*void* args = (void*) *(vm.sp-sizeof(uint64_t));
			void(*fn)() = (void(*)()) *(vm.sp-sizeof(uint64_t)*2);
			size_t siz = *vm.sp;
			printf("%s\n", args);
			printf("%x,%x : %x,%x\n", fn, &args, args, siz);

			*(vm.sp-sizeof(uint64_t)*2) = (uint64_t)__builtin_apply(fn, &args, siz);
			vm.sp -= sizeof(uint64_t)*2;*/

			// USE libffi
			*vm_ptr = vm;
			fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			return -5;
		} else if (c == COMMAND_LOAD_NATIVE_FN) {
			if (*vm.sp == 0) {
				*vm.sp_fn_ptr = dlopen;
			} else if (*vm.sp == 1) {
				*vm.sp_fn_ptr = dlerror;
			} else if (*vm.sp == 2) {
				*vm.sp_fn_ptr = dlsym;
			} else if (*vm.sp == 3) {
				*vm.sp_fn_ptr = dlclose;
			} else if (*vm.sp == 4) {
				*vm.sp_fn_ptr = puts;
			} else {
				*vm.sp = 0;
			}
		} else if (c == COMMAND_LOAD) {
			uint64_t n = 0;
			int bytes = fread(&n, 8, 1, fd);
			//printf("%d\n", n);
			if (bytes == 1) {
				//printf("%x\n", *vm.sp);

				char buffer[n];
				fread(buffer, n, 1, fd);
				memcpy((void*)*vm.sp, buffer, n);
			}
		} else if (c == COMMAND_CALL) {
			*vm_ptr = vm;
			fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			return -5;
		} else if (c == COMMAND_RET) {
			*vm_ptr = vm;
			fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			return -5;
		} else if (c >= COMMAND_JUMP_IF_ZERO && c <= COMMAND_JUMP_IF_LESS_OR_EQUAL) {
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
