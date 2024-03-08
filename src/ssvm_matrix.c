#include "ssvm_matrix.h"
#include "ssvm_op.h"

int ssvm_matrix_execute(struct vm_state* vm_ptr, FILE* fd, void* stack) {
	struct vm_state vm = *vm_ptr;

	int c = fgetc(fd);
	if (c != EOF) {
		int error_code = 0;
		ssvm_atom fn = opcode_matrix[c];
		int64_t* old_sp = vm.sp;
		vm.sp = fn(vm.sp, fd, &error_code);
		int64_t* new_sp = vm.sp;
		if (c == COMMAND_CALL) {
			printf("CALL %x %x (%ld) %x (%ld)\n",c,old_sp,*old_sp,new_sp,*new_sp);
		}
		if (c == COMMAND_CLONE || c == COMMAND_SUB || c == COMMAND_MUL) {
			printf("%x %x (%ld) %x (%ld)\n",c,old_sp,*old_sp,new_sp,*new_sp);
		}

		if (error_code != 0) {
			if (error_code == -4) {
				fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
			} else if (error_code == -5) {
				fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
			} else if (error_code == -6) {
				fprintf(stderr, "Simple Stack VM halt!\n");
			} else if (error_code == -8) {
				fprintf(stderr, "Error! Not implemented opcode: 0x%x (stub)\n", c);
			}

			*vm_ptr = vm;
			return error_code;
		}
	}
	*vm_ptr = vm;
	return 0;
}

int ssvm_matrix_call(struct vm_state vm, FILE* fd, void* stack) {
	//printf("ssvm_matrix_call(%x,%x,%x) - %d\n", &vm, fd, stack, ftell(fd));
	while (!feof(fd)) {
		int r = ssvm_matrix_execute(&vm, fd, stack);
		if (r == -42) {
			//printf("ret code: %d\n", r);
			return -42;
		}

		if (r != 0) {
			//printf("ret code: %d\n", r);
			return r;
		}
	}
	return 0;
}
