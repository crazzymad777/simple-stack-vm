#ifndef SSVM_OP_H
#define SSVM_OP_H

#include "ssvm.h"
#include <stdio.h>

typedef uint64_t* (*ssvm_operate)(uint64_t* sp, FILE* fd, int* error);

uint64_t* op_push(uint64_t* sp, FILE* fd, int* error);
uint64_t* op_pop(uint64_t* sp, FILE* fd, int* error);
uint64_t* op_print(uint64_t* sp, FILE* fd, int* error);
uint64_t* op_print_fp(union stack_pointer vm, FILE* fd, int* error);
uint64_t* op_seek_sp(union stack_pointer vm, FILE* fd, int* error);

#endif
