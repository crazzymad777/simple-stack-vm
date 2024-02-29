#ifndef SSVM_MATRIX_H
#define SSVM_MATRIX_H

#include "ssvm.h"
#include <stdio.h>

int ssvm_matrix_execute(struct vm_state* vm_ptr, FILE* fd, void* stack);
int ssvm_matrix_call(struct vm_state vm, FILE* fd, void* stack);

#endif
