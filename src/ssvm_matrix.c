#include "ssvm_matrix.h"
#include "ssvm_op.h"

int ssvm_matrix_execute(struct vm_state* vm_ptr, FILE* fd, void* stack) {
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
	while (!feof(fd)) {
		int r = ssvm_matrix_execute(&vm, fd, stack);
		if (r != 0) {
			return r;
		}
	}
	return 0;
}
