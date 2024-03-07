#include "ssvm_branches.h"

#include <string.h>
#include <dlfcn.h>
#include <math.h>

int ssvm_branches_execute(struct vm_state* vm_ptr, FILE* fd, void* stack) {
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
			**vm.sp_ptr = *(vm.sp-sizeof(uint64_t));
			vm.sp -= sizeof(uint64_t) * 2;
		} else if (c == COMMAND_PRINT) {
			printf("%lx\n", *vm.sp);
		} else if (c == COMMAND_PRINT_FP) {
			printf("%lf\n", *((double*)vm.sp));
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
			if (bytes == 1) {
				char buffer[n];
				fread(buffer, n, 1, fd);
				memcpy((void*)*vm.sp, buffer, n);
			}
		} else if (c == COMMAND_ASSERT) {
            uint64_t* x = (vm.sp-sizeof(uint64_t));
            uint64_t* y = vm.sp;
            if (*x != *y) {
                printf("%ld != %ld\n", *x, *y);
                *vm_ptr = vm;
                return -16;
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

int ssvm_branches_call(struct vm_state vm, FILE* fd, void* stack) {
	while (!feof(fd)) {
		int r = ssvm_branches_execute(&vm, fd, stack);
		if (r != 0) {
			return r;
		}
	}
	return 0;
}
