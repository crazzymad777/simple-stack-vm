#ifndef SSVM_OP_H
#define SSVM_OP_H

#include "ssvm.h"
#include <stdio.h>

typedef void* (*ssvm_operate)(void* sp, FILE* fd, int* error);

void* op_push(void* sp, FILE* fd, int* error);
void* op_pop(void* sp, FILE* fd, int* error);
void* op_print(void* sp, FILE* fd, int* error);
void* op_print_fp(void* sp, FILE* fd, int* error);
void* op_seek_sp(void* sp, FILE* fd, int* error);

#endif
