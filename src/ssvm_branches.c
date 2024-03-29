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
				vm.sp += 1;
				*vm.sp = piece;
			}
		} else if (c == COMMAND_POP) {
			**vm.sp_ptr = *(vm.sp-1);
			vm.sp -= 1 * 2;
		} else if (c == COMMAND_PRINT) {
			printf("%ld\n", *vm.sp);
		} else if (c == COMMAND_PRINT_FP) {
			printf("%lf\n", *((double*)vm.sp));
		} else if (c == COMMAND_SEEK_SP) {
			int64_t piece;
			int bytes = fread(&piece, 8, 1, fd);
			if (bytes == 1) {
				vm.sp += piece;
			}
		} else if (c == COMMAND_ADD) {
			*(vm.sp-1) = *(vm.sp-1) + *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_SUB) {
			*(vm.sp-1) = *(vm.sp-1) - *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_MUL) {
			*(vm.sp-1) = *(vm.sp-1) * *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_DIV) {
			*(vm.sp-1) = *(vm.sp-1) / *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_REM) {
			*(vm.sp-1) = *(vm.sp-1) % *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_BITWISE_AND) {
			*(vm.sp-1) = *(vm.sp-1) & *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_BITWISE_OR) {
			*(vm.sp-1) = *(vm.sp-1) | *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_BITWISE_XOR) {
			*(vm.sp-1) = *(vm.sp-1) ^ *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_CLONE) {
			uint64_t cell = *vm.sp;
			vm.sp += 1;
			*vm.sp = cell;
		} else if (c == COMMAND_OMIT) {
			vm.sp -= 1;
		} else if (c == COMMAND_MALLOC) {
			*vm.sp = (uint64_t)malloc(*vm.sp);
		} else if (c == COMMAND_FREE) {
			free(vm.sp);
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_PRINT_ALL) {
			uint64_t *ptr = vm.sp;
			do {
				printf("0x%x: 0x%x\n", vm.sp-ptr, *ptr);
				ptr -= 1;
			} while(ptr != stack);

		} else if (c == COMMAND_FP_ADD) {
			*(vm.sp_f64-1) = *(vm.sp_f64-1) + *vm.sp_f64;
			vm.sp = vm.sp-1;

		} else if (c == COMMAND_FP_SUB) {
			*(vm.sp_f64-1) = *(vm.sp_f64-1) - *vm.sp_f64;
			vm.sp = vm.sp-1;

		} else if (c == COMMAND_FP_MUL) {
			*(vm.sp_f64-1) = *(vm.sp_f64-1) * *vm.sp_f64;
			vm.sp = vm.sp-1;

		} else if (c == COMMAND_FP_DIV) {
			*(vm.sp_f64-1) = *(vm.sp_f64-1) / *vm.sp_f64;
			vm.sp = vm.sp-1;

		} else if (c == COMMAND_FP_POWER) {
			*(vm.sp_f64-1) = pow(*(vm.sp_f64-1), *vm.sp_f64);
			vm.sp = vm.sp-1;

		} else if (c == COMMAND_FP_CEIL) {
			*vm.sp_f64 = ceil(*vm.sp_f64);
		} else if (c == COMMAND_FP_ROUND) {
			*vm.sp_f64 = round(*vm.sp_f64);
		} else if (c == COMMAND_TAKE) {
			*vm.sp = **vm.sp_ptr;
		} else if (c == COMMAND_RIGHT_SHIFT) {
			*(vm.sp-1) = *(vm.sp-1) >> *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_LEFT_SHIFT) {
			*(vm.sp-1) = *(vm.sp-1) << *vm.sp;
			vm.sp = vm.sp-1;
		} else if (c == COMMAND_SWAP) {
			uint64_t value = *vm.sp;
			*vm.sp = *(vm.sp-1);
			*(vm.sp-1) = value;
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
		} else if (c == COMMAND_JUMP_IF_ZERO) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_s == 0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JUMP_IF_NON_ZERO) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_s != 0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JUMP_IF_GREAT) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_s > 0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JUMP_IF_LESS) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_s < 0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JUMP_IF_GREAT_OR_EQUAL) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_s >= 0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
		} else if (c == COMMAND_JUMP_IF_LESS_OR_EQUAL) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_s <= 0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
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
            uint64_t* x = (vm.sp-1);
            uint64_t* y = vm.sp;
            if (*x != *y) {
                printf("%ld != %ld\n", *x, *y);
                *vm_ptr = vm;
                return -16;
            }
        } else if (c == COMMAND_READ_CHAR) {
			int new_char = getchar();
			vm.sp += 1;
			*vm.sp = new_char;
		} else if (c == COMMAND_READ_INTEGER) {
			int64_t new_int;
			scanf("%ld", &new_int);
			vm.sp += 1;
			*vm.sp = new_int;
		} else if (c == COMMAND_READ_FLOATING) {
			double new_double;
			scanf("%lf", &new_double);
			vm.sp += 1;
			*vm.sp_f64 = new_double;
		} else if (c == COMMAND_READ_BINARY_INTEGER) {
			int64_t new_int;
			fread(&new_int, sizeof(new_int), 1, stdin);
			vm.sp += sizeof(new_int);
			*vm.sp = new_int;
		} else if (c == COMMAND_READ_BINARY_FLOATING) {
			double new_double;
			fread(&new_double, sizeof(new_double), 1, stdin);
			vm.sp += sizeof(new_double);
			*vm.sp_f64 = new_double;
		} else if (c == COMMAND_PRINT_STRING) {
			printf("%s", (char*)*vm.sp);
		} else if (c == COMMAND_PRINT_CHAR) {
			putchar(*vm.sp);
		} else if (c == COMMAND_IS_NAN) {
			*vm.sp = isnan(*vm.sp_f64);
		} else if (c == COMMAND_EOF) {
			*vm.sp = feof(stdin);
		} else if (c == COMMAND_CALL) {
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

			struct vm_state vm_callee;
			vm_callee.sp = vm.sp;
			int exit_code = ssvm_branches_call(vm_callee, fd, vm.sp);
			if (exit_code == -42) {
				exit_code = 0;
			}
			fseek(fd, pos + 9, SEEK_SET);
			*vm_ptr = vm;
			return exit_code;
		} else if (c == COMMAND_COMPARE_FP) {
			uint64_t value = 0;
			// char equal = *(vm.sp_f64-1) == *vm.sp_f64;
			char greater = *(vm.sp_f64-1) > *vm.sp_f64;
			char less = *(vm.sp_f64-1) < *vm.sp_f64;
			char not_equal = *(vm.sp_f64-1) != *vm.sp_f64;

			value = not_equal | (greater << 1) | (less << 2);
			// Equal - 0
			// Not Equal - 1
			// Greater - 3
			// Less - 5

			// NaN != NaN -> 1
			// NaN != x -> 1
			vm.sp_f64 += 1;
			*vm.sp = value;
		} else if (c == COMMAND_RET) {
			// *vm_ptr = vm;
			return -42;
		} else if (c == COMMAND_JUMP_IF_ZERO_FP) {
			long pos = ftell(fd) - 1;
			uint64_t offset;
			int bytes = fread(&offset, 8, 1, fd);
			if (bytes == 1) {
				if (*vm.sp_f64 == 0.0) {
					if (offset == 0) {
						fprintf(stderr, "Simple Stack VM halt!\n");
						return -6;
					}
					fseek(fd, pos + offset, SEEK_SET);
				}
			}
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
		if (r == -42) {
			return -42;
		}

		if (r != 0) {
			return r;
		}
	}
	return 0;
}
