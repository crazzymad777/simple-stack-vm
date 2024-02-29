#ifndef SSVM_BRANCHES_H
#define SSVM_BRANCHES_H

#include "ssvm.h"
#include <stdio.h>

int ssvm_branches_execute(struct vm_state* vm_ptr, FILE* fd, void* stack);
int ssvm_branches_call(struct vm_state vm, FILE* fd, void* stack);

#endif
